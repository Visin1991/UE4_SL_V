// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../UI/MenuInterface.h"
#include "TAInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOLARLAND_API UTAInstance : public UGameInstance, public IMenuInterface
{
	//Inject the IMenuInterface to our UTAInstance

	GENERATED_BODY()
public:
	UTAInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

	UFUNCTION(Exec)
	void TAHost();

	void Server();

	UFUNCTION(Exec)
	void TAJoint(const FString& address);

	UFUNCTION(Exec)
	void TAJointVisin();

	UFUNCTION(Exec)
	void TAFindFirstLocalPlayer();

	UFUNCTION(Exec,BlueprintCallable)
	void TALoadMainMenu();

private:

	TSubclassOf<class UUserWidget> m_MainMenuClass;
	class UMainMenu* m_mainMenuInstance;
};
