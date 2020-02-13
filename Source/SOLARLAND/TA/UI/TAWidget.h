// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TAWidget.generated.h"



/**
 * 
 */
UCLASS()
class SOLARLAND_API UTAWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;


	//----------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = Network)
	void TA_RefreshServerList();			    //all TA specific Blueprint function should start with TA prefix

	FDelegateHandle OnFindSessionsCompleteHandle;

	UFUNCTION(BlueprintImplementableEvent)
	void TA_OnFindSessionsComplete_BP();	    //all TA specific Blueprint function should start with TA prefix

	void OnFindSessionsComplete_SetServerNames(TArray<FString> _names);
	
	TArray<FString> m_ServerNames;

	UFUNCTION(BlueprintCallable, Category = Network)
	TArray<FString> GetServerNames();
	//----------------------------------------------------------------------------------
	
};
