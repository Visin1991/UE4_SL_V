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
	void TAServer();

	void Server();

	void JoinServer(const FString& _address);

	UFUNCTION(Exec)
	void TAJoin(const FString& address);

	UFUNCTION(Exec)
	void TAJoinVisin();

	UFUNCTION(Exec)
	void TAFindFirstLocalPlayer();

	UFUNCTION(Exec,BlueprintCallable)
	void TALoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void TALoadInGameMenu();

	FORCEINLINE void LoadMainMenu(){ TALoadMainMenu();}

private:

	TSubclassOf<class UUserWidget> m_MainMenuClass;
	TSubclassOf<class UUserWidget> m_InGameMenuClass;

	class UMainMenu* m_mainMenuInstance;
	class UMenuWidget* m_inGameMenuInstance;
};
