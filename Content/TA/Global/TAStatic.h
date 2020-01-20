// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TAStatic.generated.h"


#define V_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )
#define V_CUR_LINE  (FString::FromInt(__LINE__))
#define V_CUR_CLASS_LINE (V_CUR_CLASS + "(" + V_CUR_LINE + ")")
#define V_CUR_FUNCSIG (FString(__FUNCSIG__))

#define V_SCREEN_MSG(Param1) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(V_CUR_CLASS_LINE + ": " + Param1)) )
#define V_LOG(Param1) UE_LOG(LogTemp,Warning,TEXT("%s: %s"), *V_CUR_CLASS_LINE, *FString(Param1))
#define V_LOGM(FormatString , ...) UE_LOG(LogTemp,Warning,TEXT("%s: %s"), 	*V_CUR_CLASS_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ )

#define V_LOG_ERROR(Param1) UE_LOG(LogTemp,Error,TEXT("%s: %s"), *V_CUR_CLASS_LINE, *FString(Param1))


/**
 * 
 */
UCLASS()
class SOLARLAND_API UTAStatic : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = Static)
	static void TALogWarning(FString info);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyGame)
	static FVector TAMakeVector(float x, float y, float z);
	
};
