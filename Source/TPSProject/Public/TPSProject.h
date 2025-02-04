// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


//프로젝트 로그 카테고리 추가
DECLARE_LOG_CATEGORY_EXTERN ( TPS , Log , All );


//로그 출력 관련
//함수명 (줄 넘버)
#define CALLINFO (FString(__FUNCTION__) + TEXT("(Line : ") + FString::FromInt(__LINE__) + TEXT(")"))

//함수 정보
#define PRINT_CALLINFO() UE_LOG(TPS, Warning , TEXT("%s"), *CALLINFO );

//함수 정보 + 추가 내용
#define PRINT_LOG(fmt,...) UE_LOG(TPS, Warning , TEXT("%s %s"), *CALLINFO , *FString::Printf(fmt , ##__VA_ARGS__));



 
