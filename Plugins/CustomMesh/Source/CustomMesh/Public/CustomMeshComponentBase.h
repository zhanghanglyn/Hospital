#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"
#include "Components/MeshComponent.h"
#include "CustomMeshComponentBase.generated.h"

/*
尝试创建一个自定义墙体的Component
*/
UCLASS(hidecategories = (Object, Physics, Activation, "Components|Activation"), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = RenderComponent) //ClassGroup = (RenderComponent), meta = (BlueprintSpawnableComponent) )
class CUSTOMMESH_API UCustomMeshComponentBase : public UMeshComponent
{
	GENERATED_UCLASS_BODY()

public:

	//要继承的虚函数
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	//获取当前原件中材质元素的数量
	virtual int32 GetNumMaterials() const override;
	//计算组建的便捷，默认是一个大小为0的边界框或者球体
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	//调用此函数将此组件的动态数据发送到呈现线程
	virtual void SendRenderDynamicData_Concurrent() override;
	//用于为该组件创建任何呈现线程信息
	virtual void CreateRenderState_Concurrent() override;

protected:

	//存储点击位置，暂时随便写一个位置

	FVector TargetPoint = FVector::ZeroVector;

	//class FCustomMeshSceneProxy* SceneProxy;
};