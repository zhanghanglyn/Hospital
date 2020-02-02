#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EditTabsUtil.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphNode.h"
#include "Runtime/SlateCore/Public/Styling/SlateColor.h"
#include "Editor/GraphEditor/Public/SGraphNode.h"

//#include "ScenarioPin.h"
//#include "Runtime/Engine/Classes/EdGraph/EdGraph.h"

#include "ScenarioNodeNormal.generated.h"

/* 存储节点数据的部分 */
UCLASS()
class UScenarioNodeNormal : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UScenarioNodeNormal();

	//~ Begin UEdGraphNode Interface
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override { return true; };
	virtual void DestroyNode() override ;

	// called when this node is being renamed after a successful name validation
	virtual void OnRenameNode(const FString& NewName) override;

	/** 在图形编辑器或图形面板中创建表示此节点的可视小部件。如果没有实现，将使用默认的节点工厂。
		但我有点懒得写节点工厂还要注册了，就重载这个吧~
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
	FString NodeName = "NormalGraph";

	static const int32 INPUT_PIN_INDEX;
	static const int32 OUTPUT_PIN_INDEX;

	//添加一个自己的Graph
	UPROPERTY()
	class UEdGraph* InsideGraph;

protected:
	//TSharedPtr< class SInlineEditableTextBlock> InlineEditableText;
	//自身的小部件
	TSharedPtr<class SScenarioNodeNormal> SNodeWidgetShared;

	//是否可以双击打开Graph
	bool bOpenGraph = true;
};

/* 继承SBorder的Node显示类 */
class SScenarioNodeNormal : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SScenarioNodeNormal)
		: _NodeBgColor(FLinearColor::Gray)
		, _CategoryTEXT(FText::FromString(L"标准节点"))
		, _BUseCategoryTitle(true)
	{
	}
	SLATE_ATTRIBUTE(FSlateColor, NodeBgColor)
	SLATE_ATTRIBUTE(FText, CategoryTEXT)
	SLATE_ATTRIBUTE(bool , BUseCategoryTitle)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs , UEdGraphNode* InNode);

	//重写一个DoubleClick
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

	/*更新Node已匹配Data更新*/
	virtual void UpdateGraphNode();
	
	/*创建Pin UI*/
	virtual void CreatePinWidgets();

	/* 把一个Pin添加到Node上，必须是新创建的Pin */
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd);

/*外部方法*/
public:
	virtual void UpdateNodeNmae( FString NodeName );

protected:
	class UScenarioNodeNormal* OwnerGraphNode;


//slate显示相关参数
protected:
	TAttribute<FSlateColor> NodeBgColor;

	//该节点啊类型的名字
	FText NodeTitleName;// = FText::FromString("标准节点");

	//节点上面的类型title控件
	TSharedPtr<class SVerticalBox> CategoryTitleCtl;

	//是否需要类型Title
	bool BNeedCategoryTitle;
};