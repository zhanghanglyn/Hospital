#pragma once

#include "CoreMinimal.h"

struct FScenarioEditorTabsUtil
{
	// Details的通用ID
	static const FName GraphDetailsID;
	static const FName SearchID;

	// Editor的通用ID
	static const FName GraphEditorID;
};

//所有节点以及Pin使用的名称定义
struct FScenarioNodeUtil
{
	static const FName NodeCategoryNormal;
	static const FName NodeCategoryRoot;
	static const FName NodeCategoryEnd;

	/* 具体节点相关 */
	static const FName NodeCategoryAction;	   //最普通的事件节点
	static const FName NodeCategoryAssembly;   //装配节点
	static const FName NodeCategoryFunction;   //函数节点
};

struct FScenarioPinUtil
{
	static const FName SubPinCategoryNormal;	//普通节点的SubPin，暂时只用这一个，用来进行PinFactory的创建
	static const FName SubPinCategoryRoot;		//Root和End是特殊不能单独连接的
	static const FName SubPinCategoryEnd;		//Root和End是特殊不能单独连接的

	static const FName PinCategoryNotAllow;	//不允许任何连接
	static const FName PinCategoryNormal;	//普通Pin，只允许一个链接
	static const FName PinCategoryMulti;	//允许链接多个Pin

	static const FName PinInputBreakSourceOutput;	//只在被链接时，会断开链接来源的所有OutPut链角

	static const FName PinCategoryConnection;//中间链接的PIN

};

//控制器类型
struct FControllerCategoryUtil
{
	static const FName ControllerBase;		//基础控制器
	static const FName ControllerAction;	//Action动作控制器
	static const FName ControllerAssemble;	//装配节点控制器
};

//互动组件类型
struct FEventComponentCategoryUtil
{
	static const FName ComponentBase;		//基础组件
	static const FName ComponentShowUI;		//测试显示UI的Component
};

//互动组件对应Data类型
struct FComponentDataCategoryUtil
{
	static const FName DataCategoryBase;	//基础Data类
	static const FName DataCategoryShowUI;	//测试显示UI的ComponentData类
};