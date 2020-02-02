// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class STORYTEST_API EventEnum
{
public:
	EventEnum();
	~EventEnum();
};

enum EventType
{
	DEFAULT = 0,
	PICK_UP = 1,
};

struct SelectParam 
{
public:
	FString SelectId;
	int32 pageId;
};