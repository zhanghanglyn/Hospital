/*
	声明一个Node的数据结构体，暂时以技能树为目标设计结构
*/
#pragma once
#include "CoreMinimal.h"
#include "Core/Public/Misc/Paths.h"
#include "Runtime/Json/Public/Json.h"
#include "Core/Public/Misc/FileHelper.h"

//单个节点的数据结构
struct NodeData
{

public:
	NodeData(int32 _DataID) :DataID(_DataID) {
		ParentID = -1;
		ChildID = -1;
		C_AID = -1;
		P_AID = -1;
	};
	NodeData(NodeData* _NodeData)
	{
		DataID = _NodeData->DataID;
		ParentID = _NodeData->ParentID;
		ChildID = _NodeData->ChildID;
		Pos = _NodeData->Pos;
		LinePos = _NodeData->LinePos;
		SkillName = _NodeData->SkillName;
		C_AID = _NodeData->C_AID;
		P_AID = _NodeData->P_AID;
	};
public:
	int32 DataID = -1;		//当前节点的ID
	int32 ParentID = -1;	//父节点ID
	int32 ChildID = -1;		//子节点ID
	FVector2D Pos;			//创建位置
	FVector2D LinePos;	//连线位置
	int32 C_AID = -1;		//作为子线段ID
	int32 P_AID = -1;		//作为父线段ID

	FString SkillName;
};

//总数据,初始化时根据该数据初始化节点，存储时将所有节点数据便利并存入
struct TreeData
{
public:
	TMap<int32 , NodeData*> DataList;	//key为索引ID
};

//数据的管理类
class TreeDataMgr
{
public:
	//static TreeDataMgr* Instance;

public:
	/*static TreeDataMgr* GetInstance()
	{
		static TreeDataMgr* Instance;
		if (Instance == nullptr)
			Instance = new TreeDataMgr();

		return Instance;
	};*/

public:
	//读取所有DATA数据
	static TreeData* ReadAllData();
	//保存所有数据将数据写入本地文件
	static void SaveAllData( TreeData* _TreeData );

protected:
	//把Data包成Json
	static FString SaveToJson(TreeData* _TreeData);

protected:
	static FString FileName;// = "TreeConfig.txt";
};
