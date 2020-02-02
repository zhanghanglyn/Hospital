#include "CustomMeshComponentBase.h"
#include "Runtime/RenderCore/Public/RenderResource.h"
#include "Runtime/Engine/Public/PrimitiveSceneProxy.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Public/SceneView.h"
#include "Runtime/Engine/Public/Engine.h"

/************************************************************************/
/*	               需要在此定义缓冲区数据                               */
/************************************************************************/
class FCustomMeshVertexBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI() override 
	{
		FRHIResourceCreateInfo CreateInfo;
		VertexBufferRHI = RHICreateVertexBuffer(NumVerts * sizeof(FDynamicMeshVertex),BUF_Static,CreateInfo);
	};

	int32 NumVerts;
};

class FCustomMeshIndexBuffer : public FIndexBuffer
{
public:
	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), NumIndices * sizeof(int32),BUF_Dynamic, CreateInfo);
	};

	int32 NumIndices;
};

//顶点工厂
class FCustomMeshVertexFactory : public FLocalVertexFactory
{
public:
	FCustomMeshVertexFactory(ERHIFeatureLevel::Type InFeatureLevel, const char* InDebugName):FLocalVertexFactory(InFeatureLevel, InDebugName){};

	void Init(const FCustomMeshVertexBuffer* VertexBuffer)
	{
		if (IsInRenderingThread())
		{
			//FDataType的基类，其实就包含了UV、法线啥的乱七八糟的，就是正常Shader使用的数据了
			FDataType MeshData;
			//添加Position数据
			MeshData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
				VertexBuffer,FDynamicMeshVertex, Position, VET_Float3
			);
			//添加UV数据
			MeshData.TextureCoordinates.Add(
				FVertexStreamComponent( VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate),sizeof(FDynamicMeshVertex),EVertexElementType::VET_Float2 )
			);
			MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
				VertexBuffer , FDynamicMeshVertex, TangentX , VET_PackedNormal
			);
			MeshData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
				VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal
			);

			SetData(MeshData);
			//BeginInitResource(&CustomVertexFactory);
			FLocalVertexFactory::InitResource();
		}
		//如果不在渲染线程，用指令压进渲染线程
		else
		{
			FCustomMeshVertexFactory* VerFactory = this;
			ENQUEUE_RENDER_COMMAND(InitCableVertexFactory)(
				[VerFactory, VertexBuffer](FRHICommandListImmediate& RHICmdList)
				{
					FDataType MeshData;
					//添加Position数据
					MeshData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
						VertexBuffer, FDynamicMeshVertex, Position, VET_Float3
					);
					//添加UV数据
					MeshData.TextureCoordinates.Add(
						FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), EVertexElementType::VET_Float2)
					);
					MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
						VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal
					);
					MeshData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
						VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal
					);

					VerFactory->SetData(MeshData);
					VerFactory->InitResource();
				}
			);
			
		}
	}
};

/************************************************************************/
/*							创建Proxy相关                               */
/************************************************************************/
//自定义一个给渲染线程调用时传递的参数
struct FCustomMeshDynamicData
{
public:
	TArray<FVector> Positions;
};

class FCustomMeshSceneProxy : public FPrimitiveSceneProxy
{
public:
	FCustomMeshSceneProxy(UCustomMeshComponentBase* InComponent) : FPrimitiveSceneProxy(InComponent)
		, MaterialRelevance( InComponent->GetMaterialRelevance(GetScene().GetFeatureLevel()) )
		, VertexFactory(GetScene().GetFeatureLevel(), "FCustomMeshSceneProxy")
	{
		//这里先准备一次资源，之后根据线程的每次调用来准备资源
		//CustomVertexBuffer.NumVerts = GetRequiredVertexCount();
		VertexBuffers.InitWithDummyData(&VertexFactory, GetRequiredVertexCount());
		CustomIndexBuffer.NumIndices = GetRequiredIndicesCount();

		//CustomVertexFactory.Init(&CustomVertexBuffer);

		BeginInitResource(&CustomIndexBuffer);
		//BeginInitResource(&CustomVertexBuffer);
		//BeginInitResource(&CustomVertexFactory);

		Material = InComponent->GetMaterial(0);
		if(Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(EMaterialDomain::MD_Surface);
		}
	}

	virtual ~FCustomMeshSceneProxy()
	{
		//CustomVertexBuffer.ReleaseResource();
		CustomIndexBuffer.ReleaseResource();
		//CustomVertexFactory.ReleaseResource();
		VertexBuffers.PositionVertexBuffer.ReleaseResource();
		VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
		VertexBuffers.ColorVertexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();

		FPrimitiveSceneProxy::~FPrimitiveSceneProxy();
	}

	//获取绘制元素
	virtual void GetDynamicMeshElements(const TArray<const FSceneView *>& Views, const FSceneViewFamily& ViewFamily,
		uint32 VisibilityMap, class FMeshElementCollector& Collector) const override
	{

		FBox TestDynamicBox = FBox(FVector(-100.0f + TargetPoint.X), FVector(100.0f + TargetPoint.Y));
		DrawWireBox(
			Collector.GetPDI(0),
			GetLocalToWorld(),
			TestDynamicBox,
			FLinearColor::Yellow,
			ESceneDepthPriorityGroup::SDPG_Foreground,
			2
		);

		return;

		QUICK_SCOPE_CYCLE_COUNTER(STAT_FCustomMeshSceneProxy_GetDynamicMeshElements);

		//是否允许调试视图模式（线框）
		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);
		//上面到现在都是创建一个线框模型代理
		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		//创建一个材质渲染代理
		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
			MaterialProxy = WireframeMaterialInstance;
		else
			MaterialProxy = Material->GetRenderProxy();

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &CustomIndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				//之前已经将其设置为线框或是默认的或是Component上的了
				Mesh.MaterialRenderProxy = MaterialProxy;
				//BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(
				//	GetLocalToWorld(),GetBounds(),GetLocalBounds(), GetLocalBounds() ,true, true
				//);
				bool bHasPrecomputedVolumetricLightmap;
				FMatrix PreviousLocalToWorld;
				int32 SingleCaptureIndex;
				bool bOutputVelocity;
				GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex, bOutputVelocity);

				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, DrawsVelocity(), bOutputVelocity);
				BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = GetRequiredIndicesCount() / 3;  //3个组成一个三角形
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = GetRequiredVertexCount();
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;

				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	//需要有一个函数来接受，渲染线程发来的设置Data的函数
	void SetDynamicData_RenderThread( FCustomMeshDynamicData* DynamicData )
	{
		check(IsInRenderingThread());

		CreateMeshData(DynamicData);
	}

	//创建渲染线程调用的每帧更新的资源
	void CreateMeshData(FCustomMeshDynamicData* DynamicData)
	{
		TArray<FDynamicMeshVertex> Vertices;
		TArray<int32> Indices;

		//把顶点放入vertices中，再在Indices中把三角形组合关系弄好
		for (int32 count = 0; count < DynamicData->Positions.Num(); ++count)
		{
			/*FDynamicMeshVertex newvert0;
			newvert0.Position = DynamicData->Positions[count] + FVector(-100, 100, 0);
			FDynamicMeshVertex newvert1;
			newvert1.Position = DynamicData->Positions[count] + FVector(100, 100, 0);
			FDynamicMeshVertex newvert2;
			newvert2.Position = DynamicData->Positions[count] + FVector(-100, -100, 0);
			FDynamicMeshVertex newvert3;
			newvert3.Position = DynamicData->Positions[count] + FVector(100, -100, 0);

			Vertices.Add(newvert0);
			Vertices.Add(newvert1);
			Vertices.Add(newvert2);
			Vertices.Add(newvert3);

			Indices.Add(count* 4 + 0);
			Indices.Add(count * 4 + 1);
			Indices.Add(count * 4 + 2);
			Indices.Add(count * 4 + 3);*/
			FDynamicMeshVertex newvert0;
			newvert0.Position = DynamicData->Positions[count] + FVector(-100, 100, 50);
			FDynamicMeshVertex newvert1;
			newvert1.Position = DynamicData->Positions[count] + FVector(100, 100, 50);
			FDynamicMeshVertex newvert2;
			newvert2.Position = DynamicData->Positions[count] + FVector(-100, -100, 50);
			FDynamicMeshVertex newvert3;
			newvert3.Position = DynamicData->Positions[count] + FVector(100, -100, 50);
			FDynamicMeshVertex newvert4;
			newvert4.Position = DynamicData->Positions[count] + FVector(-100, 100, -50);
			FDynamicMeshVertex newvert5;
			newvert5.Position = DynamicData->Positions[count] + FVector(100, 100, -50);
			FDynamicMeshVertex newvert6;
			newvert6.Position = DynamicData->Positions[count] + FVector(-100, -100, -50);
			FDynamicMeshVertex newvert7;
			newvert7.Position = DynamicData->Positions[count] + FVector(100, -100, -50);

			Vertices.Add(newvert0);
			Vertices.Add(newvert1);
			Vertices.Add(newvert2);
			Vertices.Add(newvert3);
			Vertices.Add(newvert4);
			Vertices.Add(newvert5);
			Vertices.Add(newvert6);
			Vertices.Add(newvert7);

			//上面
			Indices.Add(count * 4 + 0);
			Indices.Add(count * 4 + 1);
			Indices.Add(count * 4 + 2);
			//Indices.Add(count * 4 + 2);
			//Indices.Add(count *  4 + 3);
			//Indices.Add(count * 4 + 0);
			//右面
			Indices.Add(count * 4 + 0);
			Indices.Add(count * 4 + 1);
			Indices.Add(count * 4 + 5);
			/*Indices.Add(count * 4 + 1);
			Indices.Add(count * 4 + 4);
			Indices.Add(count * 4 + 5);
			//后面
			Indices.Add(count * 4 + 0);
			Indices.Add(count * 4 + 7);
			Indices.Add(count * 4 + 4);
			Indices.Add(count * 4 + 0);
			Indices.Add(count * 4 + 3);
			Indices.Add(count * 4 + 7);
			//前面
			Indices.Add(count * 4 + 1);
			Indices.Add(count * 4 + 2);
			Indices.Add(count * 4 + 6);
			Indices.Add(count * 4 + 1);
			Indices.Add(count * 4 + 6);
			Indices.Add(count * 4 + 5);
			//左面
			Indices.Add(count * 4 + 2);
			Indices.Add(count * 4 + 3);
			Indices.Add(count * 4 + 7);
			Indices.Add(count * 4 + 2);
			Indices.Add(count * 4 + 6);
			Indices.Add(count * 4 + 7);
			//下面
			Indices.Add(count * 4 + 4);
			Indices.Add(count * 4 + 7);
			Indices.Add(count * 4 + 5);
			Indices.Add(count * 4 + 6);
			Indices.Add(count * 4 + 7);
			Indices.Add(count * 4 + 5);*/

		}
		for (int i = 0; i < Vertices.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector(), Vertex.GetTangentY(), Vertex.TangentZ.ToFVector());
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
			VertexBuffers.ColorVertexBuffer.VertexColor(i) = FColor(133,202,183);// Vertex.Color;
		}

		//当设置好了顶点和Indices后，把数据拷贝到VertexBuffer和IndexBuffer中  GetData():就是获取第一个数组的指针，也可以使用&Vertices[0]
		/*void* Buffer = RHILockVertexBuffer(CustomVertexBuffer.VertexBufferRHI, 0, Vertices.Num() * sizeof(FDynamicMeshVertex), RLM_WriteOnly);
		FMemory::Memcmp(Buffer, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
		RHIUnlockVertexBuffer(CustomVertexBuffer.VertexBufferRHI);

		void* IndexBufferData = RHILockIndexBuffer(CustomIndexBuffer.IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcmp(IndexBufferData, &Indices[0], Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(CustomIndexBuffer.IndexBufferRHI);*/
		{
			auto& VertexBuffer = VertexBuffers.PositionVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.ColorVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTangentSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTangentData(), VertexBuffer.GetTangentSize());
			RHIUnlockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTexCoordSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTexCoordData(), VertexBuffer.GetTexCoordSize());
			RHIUnlockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI);
		}

		void* IndexBufferData = RHILockIndexBuffer(CustomIndexBuffer.IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(IndexBufferData, &Indices[0], Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(CustomIndexBuffer.IndexBufferRHI);

		//TEST
		TargetPoint = DynamicData->Positions[0];
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance relevance;
		relevance.bDrawRelevance = IsShown(View);
		relevance.bDynamicRelevance = true;
		relevance.bShadowRelevance = IsShadowCast(View);
		relevance.bRenderInMainPass = ShouldRenderInMainPass();
		relevance.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		relevance.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(relevance);

		return relevance;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

	virtual SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	//获取Vertex数量
	int32 GetRequiredVertexCount() const
	{
		return 40;
	};
	//获取Indices数量
	int32 GetRequiredIndicesCount() const
	{
		return 60;
	};

protected:
	UMaterialInterface* Material;

	//FCustomMeshVertexBuffer CustomVertexBuffer;
	FCustomMeshIndexBuffer CustomIndexBuffer;
	//FCustomMeshVertexFactory CustomVertexFactory;
	FLocalVertexFactory VertexFactory;
	FStaticMeshVertexBuffers VertexBuffers;

	FMaterialRelevance MaterialRelevance;

	//测试用保存点
	FVector TargetPoint = FVector(50,50,50);
};



/************************************************************************/
/*	               CustomMeshComponent                               */
/************************************************************************/
UCustomMeshComponentBase::UCustomMeshComponentBase(const FObjectInitializer& ObjectInitializer)
	: UMeshComponent(ObjectInitializer)
{
	CastShadow = true;
	bUseAsOccluder = true;
	bCanEverAffectNavigation = true;
	bCachedMaterialParameterIndicesAreDirty = true;
	bEnableMaterialParameterCaching = false;

	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;
}

void UCustomMeshComponentBase::OnRegister()
{
	Super::OnRegister();


	//当调用后，引擎就会调用CreateRenderState_Concurrent了
	MarkRenderDynamicDataDirty();

}

void UCustomMeshComponentBase::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MarkRenderDynamicDataDirty();

	UpdateComponentToWorld();
}

void UCustomMeshComponentBase::CreateRenderState_Concurrent()
{
	Super::CreateRenderState_Concurrent();

	SendRenderDynamicData_Concurrent();
}


//向渲染线程压如我们的自定义Data
void UCustomMeshComponentBase::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		//暂时只加入一个，之后把这个提出来做成一个单独的函数
		FCustomMeshDynamicData* DynamicData = new FCustomMeshDynamicData();
		DynamicData->Positions.AddUninitialized(1);
		for (int32 PointIdx = 0; PointIdx < 1; PointIdx++)
		{
			DynamicData->Positions[PointIdx] = TargetPoint;
		}

		FCustomMeshSceneProxy* CommandProxy = (FCustomMeshSceneProxy*)SceneProxy;
		ENQUEUE_RENDER_COMMAND(FSendCustomComponentDynamicData)(
			[CommandProxy, DynamicData](FRHICommandListImmediate& RHICmdList)
			{
				CommandProxy->SetDynamicData_RenderThread(DynamicData);
			}
		);
	}
}

#pragma optimize("",off)
FPrimitiveSceneProxy* UCustomMeshComponentBase::CreateSceneProxy()
{
	//SceneProxy = new FCustomMeshSceneProxy(this);
	//return SceneProxy;
	ERHIFeatureLevel::Type InFeatureLevel = GetScene()->GetFeatureLevel();
	return new FCustomMeshSceneProxy(this);
}
#pragma optimize("",on)

int32 UCustomMeshComponentBase::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds UCustomMeshComponentBase::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = FVector::ZeroVector;
	NewBounds.BoxExtent = FVector(HALF_WORLD_MAX, HALF_WORLD_MAX, HALF_WORLD_MAX);
	NewBounds.SphereRadius = FMath::Sqrt(3.0f * FMath::Square(HALF_WORLD_MAX));
	return NewBounds;
}