// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../UI/MenuInterface.h"
#include "OnlineSubsystem.h"
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
	virtual void Server();
	virtual void JoinServer(uint32 _index);
	virtual void LoadMainMenu() { TALoadMainMenu(); }
	virtual void RefreshServerList();
	virtual void OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result);

public:
	UTAInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(Exec)
	void TAServer();

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

private:

	TSubclassOf<class UUserWidget> m_MainMenuClass;
	TSubclassOf<class UUserWidget> m_InGameMenuClass;

	class UMainMenu* m_mainMenuInstance;
	class UMenuWidget* m_inGameMenuInstance;


	IOnlineSessionPtr m_sessionInterface;
	TSharedPtr<class FOnlineSessionSearch> m_sessionSearch;


	void OnCreateSessionComplete(FName _sessionName,bool _success);
	void OnDestroySessionComplete(FName _sessionName, bool _success);
	void OnFindSessionsComplete(bool _success);

	void CreateSession();
};
