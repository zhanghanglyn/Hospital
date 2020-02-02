#include "NodeDataUtil.h"

FString TreeDataMgr::FileName = "TreeConfig.txt";

void TreeDataMgr::SaveAllData(TreeData* _TreeData)
{
	
	FString testWrite = TreeDataMgr::SaveToJson(_TreeData);
	FString ConfigPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()) + "TestSlate/Config/" + FileName;

	FFileHelper::SaveStringToFile(testWrite, ConfigPath.GetCharArray().GetData());

}

FString TreeDataMgr::SaveToJson(TreeData* _TreeData)
{
	FString OutputString;

	TSharedPtr< FJsonObject> MainJsonObject = MakeShareable(new FJsonObject);

	TArray<TSharedPtr<FJsonValue>> PackagesThatCanBeRestored;

	//循环遍历每一个节点
	for (TMap<int32, NodeData*>::TConstIterator iter(_TreeData->DataList); iter; ++iter)
	{
		TSharedPtr< FJsonObject> NodeJson = MakeShareable(new FJsonObject);
		NodeJson->SetNumberField(TEXT("DataID"), iter->Value->DataID);
		NodeJson->SetNumberField("ParentID", iter->Value->ParentID);
		NodeJson->SetNumberField("ChildID", iter->Value->ChildID);
		NodeJson->SetNumberField("PosX", iter->Value->Pos.X);
		NodeJson->SetNumberField("PosY", iter->Value->Pos.Y);
		NodeJson->SetNumberField("LinePosX", iter->Value->LinePos.X);
		NodeJson->SetNumberField("LinePosY", iter->Value->LinePos.Y);
		NodeJson->SetNumberField("C_AID", iter->Value->C_AID);
		NodeJson->SetNumberField("P_AID", iter->Value->P_AID);
		NodeJson->SetStringField("SkillName", iter->Value->SkillName);

		//FString NodeId = "Node_" + FString::FromInt(iter->Value->DataID);
		//MainJsonObject->SetObjectField(NodeId, NodeJson);
		//jsonArray.Add(FJsonValue::AsString(NodeId));

		TSharedPtr<FJsonValue> EntryValue = MakeShareable(new FJsonValueObject(NodeJson));
		PackagesThatCanBeRestored.Add(EntryValue);

	}

	MainJsonObject->SetArrayField(TEXT("DataArray"), PackagesThatCanBeRestored);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), Writer);

	return OutputString;
}

TreeData* TreeDataMgr::ReadAllData()
{
	FString ConfigPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()) + "TestSlate/Config/" + FileName;
	FString JsonString;
	TreeData* ResultData = new TreeData();

	if (FFileHelper::LoadFileToString(JsonString, ConfigPath.GetCharArray().GetData()))
	{
		
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<>::Create(JsonString);
		//如果反序列化成功
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TArray<TSharedPtr<FJsonValue>> PackagesThatCanBeRestoredArray = JsonObject->GetArrayField(TEXT("DataArray"));
			for (auto It = PackagesThatCanBeRestoredArray.CreateConstIterator(); It; ++It)
			{
				TSharedPtr<FJsonObject> EntryObject = (*It)->AsObject();

				const int32 DataID = EntryObject->GetIntegerField(TEXT("DataID"));
				const int32 ParentID = EntryObject->GetIntegerField(TEXT("ParentID"));
				const int32 ChildID = EntryObject->GetIntegerField(TEXT("ChildID"));
				const int32 PosX = EntryObject->GetIntegerField(TEXT("PosX"));
				const int32 PosY = EntryObject->GetIntegerField(TEXT("PosY"));
				const int32 LinePosX = EntryObject->GetIntegerField(TEXT("LinePosX"));
				const int32 LinePosY = EntryObject->GetIntegerField(TEXT("LinePosY"));
				const int32 C_AID = EntryObject->GetIntegerField(TEXT("C_AID"));
				const int32 P_AID = EntryObject->GetIntegerField(TEXT("P_AID"));
				const FString SkillName = EntryObject->GetStringField(TEXT("SkillName"));

				//创建一个TempNode
				NodeData* TempNodeData = new NodeData(DataID);
				TempNodeData->ParentID = ParentID;
				TempNodeData->ChildID = ChildID;
				TempNodeData->Pos = FVector2D(PosX, PosY);
				TempNodeData->LinePos = FVector2D(LinePosX, LinePosY);
				TempNodeData->SkillName = SkillName;
				TempNodeData->C_AID = C_AID;
				TempNodeData->P_AID = P_AID;

				ResultData->DataList.Add(DataID ,TempNodeData);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Load Json Config Error!!!!!!!!!!!"));
	return ResultData;
}