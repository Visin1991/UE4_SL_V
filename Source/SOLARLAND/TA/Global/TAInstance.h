// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../UI/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "TAInstance.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindSessionsCompleteSignature,TArray<FString>)

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
	void TAServerTravel();

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

	//TSubclassOf<class UUserWidget> m_MainMenuClass;
	//TSubclassOf<class UUserWidget> m_InGameMenuClass;

	//class UMainMenu* m_mainMenuInstance;
	//class UMenuWidget* m_inGameMenuInstance;

private:

	//Private variables & Functions for Network

	IOnlineSessionPtr m_sessionInterface;
	//Row c++ class need manually de-allocate memory, so we use Unreal shared pointer to handle the dirty task
	TSharedPtr<class FOnlineSessionSearch> m_sessionSearch;
	void OnCreateSessionComplete(FName _sessionName, bool _success);
	void OnDestroySessionComplete(FName _sessionName, bool _success);
	void OnFindSessionsComplete(bool _success);
	void CreateSession();


public:

	//Public variables & functions for Network

	//------------------------------------------------------------------------------------------------------------
	//virtual interface is obsolete
	//------------------------------------------------------------------------------------------------------------
	virtual void Server();
	virtual void JoinServer(uint32 _index);
	virtual void LoadMainMenu() { TALoadMainMenu(); }
	virtual void RefreshServerList();
	virtual void OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result);
	//------------------------------------------------------------------------------------------------------------
	
	UFUNCTION(BlueprintCallable, Category = TA_Network)
	void CreateGame();

	UFUNCTION(BlueprintCallable, Category = TA_Network)
	void JoinGame(int _index);



	class UTAWidget* uptr_ServerMenuListener;

	UFUNCTION(BlueprintCallable, Category = Instance)
	void BindServerMenuListener(class UTAWidget* _listener);

	FOnFindSessionsCompleteSignature OnFindSessionsCompleteEvent;


private:
	static UTAInstance* m_TAInstance;
	class UTAUIManager* m_TAUIManager;


public:

	UFUNCTION(BlueprintCallable, Category = Static)
	static UTAInstance* GetTAGameInstance();
	UTAUIManager* GetUIManager();
};
