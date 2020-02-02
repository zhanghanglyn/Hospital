// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Public/PrimitiveSceneProxy.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Public/DynamicMeshBuilder.h"
#include "Engine/Classes/Materials/Material.h"
#include "RenderCore/Public/RenderResource.h"

/* �����Լ��Ķ��㻺�� */
class FCustomVertexBuffer : public FVertexBuffer
{
public:
	TArray<FDynamicMeshVertex> Vertices;


	virtual void InitRHI() override;

};

/* �����Լ����������� */
class FCustomIndexBuffer : public FIndexBuffer
{
public:
	TArray<int32> Indices;

	virtual void InitRHI() override;;

};

/* �Զ���һ�����㹤�� */
class FCustomVertexFactory : public FLocalVertexFactory
{

public:
	FCustomVertexFactory(ERHIFeatureLevel::Type InFeatureLevel = ERHIFeatureLevel::ES3_1, /* ֧�ֵ���Ⱦ���𣬱���DX11��DX10��OPENGLxxx�� */
		const char* InDebugName = "CustomProxy") : FLocalVertexFactory(InFeatureLevel, InDebugName)
	{};

	void Init(const FCustomVertexBuffer* VerTexBuffer);
	/*{
		if (IsInRenderingThread())
		{
			FDataType MeshData;
			MeshData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
				VerTexBuffer, FDynamicMeshVertex, Position, VET_Float3
			);
			MeshData.TextureCoordinates.Add(
				FVertexStreamComponent(VerTexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate),
					sizeof(FDynamicMeshVertex), EVertexElementType::VET_Float2) // UV������FLOAT2,
			);
			MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
				VerTexBuffer, FDynamicMeshVertex, TangentX,
				VET_Float3
			);
			MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
				VerTexBuffer, FDynamicMeshVertex, TangentZ,
				VET_Float3
			);
			SetData(MeshData);
		}
		// ���������Ⱦ�����У�Ҫ���ó�ʼ�����������Ⱦ�����У�����Ľ̳���д�ģ������Ҹо������÷���ͬ�п��ܲ���Ҫ
		else
		{
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
				FName(InitFCustomVertexFactory), FCustomVertexFactory*, VertexFactory, this,
				const FCustomVertexBuffer*, VerTexBuffer, VerTexBuffer,
				{
					FDataType MeshData;
					MeshData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
						VerTexBuffer, FDynamicMeshVertex, Position, VET_Float3
					);
					MeshData.TextureCoordinates.Add(
						FVertexStreamComponent(VerTexBuffer,STRUCT_OFFSET(FDynamicMeshVertex,TextureCoordinate),
						sizeof(FDynamicMeshVertex),EVertexElementType::VET_Float2) // UV������FLOAT2, 
					);
					MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
						VerTexBuffer, FDynamicMeshVertex,TangentX,
						VET_Float3
					);
					MeshData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(
						VerTexBuffer, FDynamicMeshVertex, TangentZ,
						VET_Float3
					);
					VertexFactory->SetData(MeshData);
				}
			);
		}
	}*/

};

/**
 * 
 */
class FCustomPrimitiveSceneProxy : public FPrimitiveSceneProxy
{
public:

	FCustomPrimitiveSceneProxy(const UPrimitiveComponent* InComponent, FName ResourceName = NAME_None);

	virtual ~FCustomPrimitiveSceneProxy() {

		m_VertexBuffer.ReleaseResource();
		m_IndexBuffer.ReleaseResource();
		m_VerTexFactory.ReleaseResource();

		FPrimitiveSceneProxy::~FPrimitiveSceneProxy();

	};

	virtual SIZE_T GetTypeHash() const override
	{
		return 3312;
	}

	virtual uint32 GetMemoryFootprint(void) const override {
		return (this->GetAllocatedSize() + sizeof(*this));
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView *>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override
	{
		FBox TestDynamicBox = FBox(FVector(-100.0f), FVector(100.0f));
		DrawWireBox(
			Collector.GetPDI(0),
			GetLocalToWorld(),
			TestDynamicBox,
			FLinearColor::Yellow,
			ESceneDepthPriorityGroup::SDPG_Foreground,
			2
		);
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance relevance;
		relevance.bDrawRelevance = true;// IsShown(View);
		relevance.bDynamicRelevance = true;
		relevance.bShadowRelevance = true;//IsShadowCast(View);
		relevance.bEditorPrimitiveRelevance = true;//UseEditorCompositing(View);
		//relevance.bUseCustomViewData = true;

		return relevance;
	}

	virtual void DrawStaticElements(FStaticPrimitiveDrawInterface* PDI) override;

public:
	/* ����ģ�� */
	void SetVerTex(  );

	/* ����ģ�� */
	void Update() { BeginUpdateResourceRHI(nullptr); };

public:
	FCustomVertexBuffer m_VertexBuffer;
	FCustomIndexBuffer m_IndexBuffer;
	FCustomVertexFactory m_VerTexFactory;

};

