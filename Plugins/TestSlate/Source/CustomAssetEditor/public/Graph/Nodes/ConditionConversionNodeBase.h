﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EditTabsUtil.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphNode.h"
#include "Runtime/SlateCore/Public/Styling/SlateColor.h"
#include "Editor/GraphEditor/Public/SGraphNode.h"

//#include "ScenarioPin.h"
//#include "Runtime/Engine/Classes/EdGraph/EdGraph.h"

#include "ConditionConversionNodeBase.generated.h"

/* 存储节点数据的部分 */
UCLASS()
class UConditionConversionNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UConditionConversionNodeBase();

	//~ Begin UEdGraphNode Interface
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;

	virtual bool CanUserDeleteNode() const override { return true; };
	virtual void DestroyNode() override ;  //清除该节点的时候，要断开所有Pin

	/*在编辑器中更改此节点的一个引脚的连接列表时调用*/
	virtual void PinConnectionListChanged(UEdGraphPin* Pin);

	// called when this node is being renamed after a successful name validation
	virtual void OnRenameNode(const FString& NewName) override;

	/** 
	直接通过模板创建，好像就不需要他了
	*/
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

	// 取名的时候用来验证是否可以取，暂时不处理
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const { return NULL; }
	/**
	* 粘贴节点的时候进行的操作，暂时先拜个早年吧
	*/
	virtual void PostPasteNode() { /*UE_LOG(LogTemp, Warning, TEXT(" Happy New Year"));*/ };
	//~ End

	/* 父类方法，继承后可以在里面添加新节点 */
	virtual void AllocateDefaultPins();

	/* 自定义可继承Virtual事件 */
	//双击Ndoe的事件
	virtual void DoubleClickNodeCall();

	//获取OutPutPin
	virtual UEdGraphPin* GetOutPutPin();
	//获取InPutPin
	virtual UEdGraphPin* GetInPutPin();

	//外部Detail变化时用来更新的函数
	virtual void OnDetailUpdate(const FPropertyChangedEvent& PropertyChangedEvent);

	//创建的时候调用 用来把链接节点和上下节点关联起来
	bool CreateConnections(class UScenarioNodeNormal* PreviousState, class UScenarioNodeNormal* NextState, FName TransName);


	class UScenarioNodeNormal* GetFromNode() const;
	class UScenarioNodeNormal* GetTargetNode() const;

	void SetTransitionName(FName NewName);
	FName GetTransitionName() const;

	//链接节点暂时没有这种逻辑
	//获取OutPin连接的所有Pin
	TArray<UEdGraphPin*> GetAllOutPinLinked();
	//获取OutPin链接的所有Node
	TArray<UEdGraphNode*> GetAllOutNodeLinked();
	/* 获取该Node的接入Nodes*/
	TArray<UEdGraphNode*> GetAllInputNodeLinked();

public:

	//所有Node都有的一个类型
	FName NodeCategory = FScenarioNodeUtil::NodeCategoryNormal;

	UPROPERTY(EditAnywhere, Category = "ScenarioGraphNode")
	FString NodeName = L"条件";

	static const int32 INPUT_PIN_INDEX;
	static const int32 OUTPUT_PIN_INDEX;

	//添加一个自己的Graph
	UPROPERTY()
	class UEdGraph* InsideGraph;

protected:
	//TSharedPtr< class SInlineEditableTextBlock> InlineEditableText;
	//自身的小部件
	TSharedPtr<class SConditionConversionNodeBase> SNodeWidgetShared;

	//是否可以双击打开Graph
	bool bOpenGraph = true;

	UPROPERTY()
	FName Transition;
	UPROPERTY()
	class UScenarioNodeNormal* FromNode;
	UPROPERTY()
	class UScenarioNodeNormal* TargetNode;
};

/* 继承SBorder的Node显示类 */
class SConditionConversionNodeBase : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SConditionConversionNodeBase)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs , UConditionConversionNodeBase* InNode);

	/*更新Node已匹配Data更新*/
	virtual void UpdateGraphNode() override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter) override;
	//如果此节点依赖其他节点的位置，返回True
	virtual bool RequiresSecondPassLayout() const override { return true; };
	//当require返回true的时候，执行第二遍布局
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& InNodeToWidgetLookup) const override;

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

protected:
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;
	FSlateColor GetTransitionColor() const;

public:
	virtual void UpdateNodeNmae(FString NodeName);

//slate显示相关参数
protected:
	//节点名字控件
	TSharedPtr<class STextBlock> NodeNameBlock;
};