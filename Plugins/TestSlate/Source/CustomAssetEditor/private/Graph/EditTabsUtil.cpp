#include "EditTabsUtil.h"

const FName FScenarioEditorTabsUtil::GraphDetailsID(TEXT("ScenarioEditor_Properties"));
const FName FScenarioEditorTabsUtil::SearchID(TEXT("ScenarioEditor_Search"));

const FName FScenarioEditorTabsUtil::GraphEditorID(TEXT("Document"));

/* �ڵ���� */
const FName FScenarioNodeUtil::NodeCategoryNormal(TEXT("NormalBase"));
const FName FScenarioNodeUtil::NodeCategoryRoot(TEXT("NormalRoot"));
const FName FScenarioNodeUtil::NodeCategoryEnd(TEXT("NormalEND"));
const FName FScenarioNodeUtil::NodeCategoryAssembly(TEXT("NodeAssembly"));
const FName FScenarioNodeUtil::NodeCategoryAction(TEXT("NodeAction"));
const FName FScenarioNodeUtil::NodeCategoryFunction(TEXT("NodeFunction"));

//PIN���
const FName FScenarioPinUtil::SubPinCategoryRoot(TEXT("SubRoot"));
const FName FScenarioPinUtil::SubPinCategoryEnd(TEXT("SubEnd"));
const FName FScenarioPinUtil::PinCategoryNotAllow(TEXT("NotAllowPin"));
const FName FScenarioPinUtil::PinCategoryNormal(TEXT("NormalBase"));
const FName FScenarioPinUtil::PinCategoryMulti(TEXT("NormalMulti"));
const FName FScenarioPinUtil::PinCategoryConnection(TEXT("PinConnection"));
const FName FScenarioPinUtil::PinInputBreakSourceOutput(TEXT("PinInputBreakSourceOutput"));
;

//���������
const FName FControllerCategoryUtil::ControllerBase(TEXT("ControllerBase"));
const FName FControllerCategoryUtil::ControllerAction(TEXT("ControllerAction"));
const FName FControllerCategoryUtil::ControllerAssemble(TEXT("ControllerAssemble"));

//����������
const FName FEventComponentCategoryUtil::ComponentBase(TEXT("UEventComponentBase"));
const FName FEventComponentCategoryUtil::ComponentShowUI(TEXT("UShowUIComponentBase"));

//���������ӦData���
const FName FComponentDataCategoryUtil::DataCategoryBase(TEXT("DataCategoryBase"));
const FName FComponentDataCategoryUtil::DataCategoryShowUI(TEXT("DataCategoryShowUI"));

