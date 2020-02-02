#pragma once

#include "CoreMinimal.h"

struct FScenarioEditorTabsUtil
{
	// Details��ͨ��ID
	static const FName GraphDetailsID;
	static const FName SearchID;

	// Editor��ͨ��ID
	static const FName GraphEditorID;
};

//���нڵ��Լ�Pinʹ�õ����ƶ���
struct FScenarioNodeUtil
{
	static const FName NodeCategoryNormal;
	static const FName NodeCategoryRoot;
	static const FName NodeCategoryEnd;

	/* ����ڵ���� */
	static const FName NodeCategoryAction;	   //����ͨ���¼��ڵ�
	static const FName NodeCategoryAssembly;   //װ��ڵ�
	static const FName NodeCategoryFunction;   //�����ڵ�
};

struct FScenarioPinUtil
{
	static const FName SubPinCategoryNormal;	//��ͨ�ڵ��SubPin����ʱֻ����һ������������PinFactory�Ĵ���
	static const FName SubPinCategoryRoot;		//Root��End�����ⲻ�ܵ������ӵ�
	static const FName SubPinCategoryEnd;		//Root��End�����ⲻ�ܵ������ӵ�

	static const FName PinCategoryNotAllow;	//�������κ�����
	static const FName PinCategoryNormal;	//��ͨPin��ֻ����һ������
	static const FName PinCategoryMulti;	//�������Ӷ��Pin

	static const FName PinInputBreakSourceOutput;	//ֻ�ڱ�����ʱ����Ͽ�������Դ������OutPut����

	static const FName PinCategoryConnection;//�м����ӵ�PIN

};

//����������
struct FControllerCategoryUtil
{
	static const FName ControllerBase;		//����������
	static const FName ControllerAction;	//Action����������
	static const FName ControllerAssemble;	//װ��ڵ������
};

//�����������
struct FEventComponentCategoryUtil
{
	static const FName ComponentBase;		//�������
	static const FName ComponentShowUI;		//������ʾUI��Component
};

//���������ӦData����
struct FComponentDataCategoryUtil
{
	static const FName DataCategoryBase;	//����Data��
	static const FName DataCategoryShowUI;	//������ʾUI��ComponentData��
};