#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphSchema.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraph.h"
#include "ScenarioSchemaAction.h"
#include "AssetData.h"
#include "UObject/ObjectKey.h"
#include "ScenarioNodeNormal.h"
#include "ScenarioGraphSchema.generated.h"

UCLASS()
class UScenarioGraphSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

public:
	/*
	使用任何默认节点填充新图形
	*
	* @param图形用于添加默认节点
	* @param contextclass如果指定，图形终端将使用这个类来搜索签名的上下文(即接口函数)
	*/
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const;
	/*
	获取当右键点击图形时可以进行的所有操作
	GetGraphContextActions方法可以告诉蓝图在右键时哪些类能显示，即能创建的节点类型
	*/
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const;
	/*
	确定是否可以在两个引脚之间创建连接。
	*
	* @param是第一个pin码。
	* @param b第二个引脚。
	*
	* @ @如果连接是合法的，返回一个空字符串，否则会出现描述连接失败原因的消息。
	*/
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	/**
		获取应该添加到右键单击节点或大头针上下文菜单中的操作
		*
		当前图。
		* @param ingraphnode获取上下文菜单的节点，如果有的话。
		* @param ingraphpin如果有pin，则点击它以提供额外的上下文
		要附加操作的菜单生成器。
		* @param bisdebug是当前调试会话的图形编辑器(应该禁用任何非调试命令)
	*/
	virtual void GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const override;


	/*链接两个引脚并通过中间节点 可以在此自定义中间节点*/
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;

	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const;

	/* 断开节点连接的三个方法 */
	/**
	 * Breaks all links from/to a single node
	 *
	 * @param	TargetNode	The node to break links on
	 */
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	/**
	 * Breaks all links from/to a single pin
	 *
	 * @param	TargetPin	The pin to break links on
	 * @param	bSendsNodeNotifcation	whether to send a notification to the node post pin connection change
	 */
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	/**
	 * Breaks the link between two nodes.
	 *
	 * @param	SourcePin	The pin where the link begins.
	 * @param	TargetLink	The pin where the link ends.
	 */
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;

protected:

public:
	//static const FName PC_Normal;
	//static const FName PC_Root;

};