// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPrimitiveSceneProxy.h"

FCustomPrimitiveSceneProxy::FCustomPrimitiveSceneProxy(const UPrimitiveComponent* InComponent,
	FName ResourceName /* = NAME_None */) : FPrimitiveSceneProxy(InComponent, ResourceName)
{
	//m_VerTexFactory = new FCustomVertexFactory(ERHIFeatureLevel::SM4, "TestDraw!!!");
	//SetVerTex();
}

void FCustomPrimitiveSceneProxy::SetVerTex()
{
	const float BoxSize = 50.0f;

	//填充顶点
	m_VertexBuffer.Vertices.Add(FVector(0.0f));
	m_VertexBuffer.Vertices.Add(FVector(BoxSize, 0.0f, 0.0f));
	m_VertexBuffer.Vertices.Add(FVector(0.0f, BoxSize, 0.0f));
	m_VertexBuffer.Vertices.Add(FVector(0.0f, 0.0f, BoxSize));

	//填充索引
	m_IndexBuffer.Indices.Add(0);
	m_IndexBuffer.Indices.Add(1);
	m_IndexBuffer.Indices.Add(2);

	m_IndexBuffer.Indices.Add(0);
	m_IndexBuffer.Indices.Add(1);
	m_IndexBuffer.Indices.Add(3);

	m_IndexBuffer.Indices.Add(0);
	m_IndexBuffer.Indices.Add(3);
	m_IndexBuffer.Indices.Add(2);

	m_IndexBuffer.Indices.Add(2);
	m_IndexBuffer.Indices.Add(3);
	m_IndexBuffer.Indices.Add(1);

	//初始化
	m_VerTexFactory.Init(&m_VertexBuffer);
	BeginInitResource(&m_VertexBuffer);
	BeginInitResource(&m_IndexBuffer);
	BeginInitResource(&m_VerTexFactory);

}

/* 绘制静态渲染模型 */
void FCustomPrimitiveSceneProxy::DrawStaticElements(FStaticPrimitiveDrawInterface* PDI)
{
	FMeshBatch Mesh;
	FMeshBatchElement& BatchElement = Mesh.Elements[0];
	BatchElement.IndexBuffer = &m_IndexBuffer;
	Mesh.bWireframe = false;
	Mesh.VertexFactory = &m_VerTexFactory;
	/* MD_Surface传入这个参数相当于计算默认的3D面的材质 */
	Mesh.MaterialRenderProxy = UMaterial::GetDefaultMaterial(MD_Surface)->GetRenderProxy();
	BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(
		GetLocalToWorld(),GetBounds(),GetLocalBounds(), GetLocalBounds(),true,true
	);
	BatchElement.FirstIndex = 0;
	BatchElement.NumPrimitives = m_IndexBuffer.Indices.Num() / 3;
	BatchElement.MinVertexIndex = 0;
	BatchElement.MaxVertexIndex = m_IndexBuffer.Indices.Num() / 3 - 1;
	Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
	Mesh.Type = PT_TriangleList;
	Mesh.DepthPriorityGroup = SDPG_Foreground;
	Mesh.bCanApplyViewModeOverrides = false;
	Mesh.bDisableBackfaceCulling = false;
	PDI->DrawMesh(Mesh , 1.0f);

}






/* 自定义顶点缓冲相关 */
void FCustomVertexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* LockedData = nullptr;
	VertexBufferRHI = RHICreateAndLockVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex),
		BUF_Static , CreateInfo, LockedData);

	FMemory::Memcmp(LockedData, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
	RHIUnlockVertexBuffer(VertexBufferRHI);
}

/* 自定义索引缓冲相关 */
void FCustomIndexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* LockedData = nullptr;
	IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32),
		BUF_Static, CreateInfo);
	void* Buffer = RHILockIndexBuffer(IndexBufferRHI, 0, sizeof(int32), RLM_WriteOnly);

	FMemory::Memcmp(LockedData, Indices.GetData(), Indices.Num() * sizeof(int32));
	RHIUnlockIndexBuffer(IndexBufferRHI);

}

/* 工厂相关 */
void FCustomVertexFactory::Init(const FCustomVertexBuffer* VerTexBuffer)
{
	if (IsInRenderingThread())
	{
		FDataType MeshData;
		MeshData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
			VerTexBuffer, FDynamicMeshVertex, Position, VET_Float3
		);
		MeshData.TextureCoordinates.Add(
			FVertexStreamComponent(VerTexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate),
				sizeof(FDynamicMeshVertex), EVertexElementType::VET_Float2) // UV所以是FLOAT2,
		);
		MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
			VerTexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal
			//VET_Float3
		);
		//MeshData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer,FDynamicMeshVertex,Tan,VET_PackedNormal)
		MeshData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
			VerTexBuffer, FDynamicMeshVertex, TangentZ,
			VET_PackedNormal
		);
		SetData(MeshData);
	}
	// 如果不在渲染进程中，要将该初始化代码加入渲染进程中（大象的教程中写的，不过我感觉根据用法不同有可能不需要
	else
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER( InitCustomVertexFactor
			/*FName(InitFCustomVertexFactory)*/, FCustomVertexFactory*, VertexFactory, this,
			const FCustomVertexBuffer*, VerTexBuffer, VerTexBuffer,
			{
				FDataType MeshData;
				MeshData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
					VerTexBuffer, FDynamicMeshVertex, Position, VET_Float3
				);
				MeshData.TextureCoordinates.Add(
					FVertexStreamComponent(VerTexBuffer,STRUCT_OFFSET(FDynamicMeshVertex,TextureCoordinate),
					sizeof(FDynamicMeshVertex),EVertexElementType::VET_Float2) // UV所以是FLOAT2, 
				);
				MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
					VerTexBuffer, FDynamicMeshVertex,TangentX,
					VET_PackedNormal
				);
				MeshData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
					VerTexBuffer, FDynamicMeshVertex, TangentZ,
					VET_PackedNormal
				);
				VertexFactory->SetData(MeshData);
			}
		);
	}
}