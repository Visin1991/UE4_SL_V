// Fill out your copyright notice in the Description page of Project Settings.


#include "TAInstance.h"
#include "TAStatic.h"
#include "GameFramework/PlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../UI/MainMenu.h"
#include "../UI/MenuWidget.h"


#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"



/*
	1. Load The main menu UI Blueprint

	2. TAMain map Level Blue print----> Get TAInstance  ---> Call TALoadMainMenu()
		Inject TAInstance -->To--> MainMenu
				MainMenu::Server()-->Call--->TAInstance::Server()
		
*/

//------------------------------------------------------------------------------------------------
// Session Creation

// void UTAInstance::Init() {......}        //config session callback functions......

// void UTAInstance::TAServer() {......}    //TAServer implement the ServerTravel ....

// void UTAInstance::Server() {......}      //Server Function will create a session for us

//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// Session Creation & Destroy & SessionSearch

// 在Session Creation 阶段我们需要对Session 进行配置：是否进行LAN匹配，链接数量，是否曝光

// 在Session Find 阶段我们也需要对SessionSearch 进行配置：是否进行LAN匹配，第三方SDK。。。。。。等等

//------------------------------------------------------------------------------------------------

#define SESSION_NAME TEXT("My SessionGame")


UTAInstance::UTAInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> bp_MainMenu(TEXT("/Game/TA/Global/UI/WBP_Main"));
	if (!ensure(bp_MainMenu.Class != nullptr))return;
	m_MainMenuClass = bp_MainMenu.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> bp_InGameMenu(TEXT("/Game/TA/Global/UI/WBP_InGameMenu"));
	if (!ensure(bp_InGameMenu.Class != nullptr))return;
	m_InGameMenuClass = bp_InGameMenu.Class;
}

void UTAInstance::Init()
{
	IOnlineSubsystem* subsystem =  IOnlineSubsystem::Get();
	if (subsystem != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Found Subsystem %s"),*subsystem->GetSubsystemName().ToString());
		m_sessionInterface = subsystem->GetSessionInterface();
		if (m_sessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Get A SessionInterface"));
			m_sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTAInstance::OnCreateSessionComplete);
			m_sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UTAInstance::OnDestroySessionComplete);
			m_sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTAInstance::OnFindSessionsComplete);
			m_sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTAInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		V_LOG("Could Not found subsystem");
	}
}

//====================================================================================
//UIManager

void UTAInstance::Server()
{
	if (m_sessionInterface.IsValid())
	{
		auto existingSession = m_sessionInterface->GetNamedSession(SESSION_NAME);
		if (existingSession != nullptr)
		{
			//If the session already exist, we will destroy it. And the the destroy session success,we will create the session again
			m_sessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			//If the session not exist, we create the session directly
			CreateSession();
		}
	}
}

void UTAInstance::JoinServer(uint32 _index)
{
	V_LOG("JoinServer Function Get Call......");

	//TAJoin(_address);
	if (!m_sessionInterface.IsValid()) { V_LOG("m_sessionInterface is nullptr"); return; }
	if (!m_sessionSearch.IsValid()) { V_LOG("m_sessionSearch is not valid"); return; }

	if (m_mainMenuInstance != nullptr)
	{
		m_mainMenuInstance->TearDown();
	}

	m_sessionInterface->JoinSession(0, SESSION_NAME, m_sessionSearch->SearchResults[_index]);

}

void UTAInstance::RefreshServerList()
{
	m_sessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (m_sessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Find Session"));
		m_sessionSearch->bIsLanQuery = true;
		//m_sessionSearch->QuerySettings.Set()
		m_sessionInterface->FindSessions(0, m_sessionSearch.ToSharedRef());
	}
}

//=====================================================================================

void UTAInstance::OnCreateSessionComplete(FName _sessionName, bool _success)
{
	if (!_success) {
		UE_LOG(LogTemp,Warning,TEXT("Could not create session"));
		return;
	}

	UE_LOG(LogTemp,Warning,TEXT("OnCreateSession Complete...... Start ServerTravel"));
	TAServer();
}

void UTAInstance::OnDestroySessionComplete(FName _sessionName, bool _success)
{
	if (_success)
	{
		CreateSession();
	}
}

void UTAInstance::OnFindSessionsComplete(bool _success)
{
	if (!_success)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Find Session Complete....Failed...."));
		return;
	}

	UE_LOG(LogTemp,Warning,TEXT("On Find Session Complete......Success"));

	if (!m_sessionSearch.IsValid())
	{
		UE_LOG(LogTemp,Warning,TEXT("SessionSearch is not Valid"));
	}

	//Here we should have a function pointer. to let our other manager to bind it
	//currently we can just hard code .

	TArray<FString> serverNames;
		
	for (auto& searchResult : m_sessionSearch->SearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *searchResult.GetSessionIdStr());
		serverNames.Add(searchResult.GetSessionIdStr());
	}

	if (m_mainMenuInstance != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Find %d servers"),serverNames.Num());
		m_mainMenuInstance->SetServerList(serverNames);
	}
	
}

void UTAInstance::OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type _result)
{

	if (!m_sessionInterface.IsValid()) {
		V_LOG("m_sessionInterface is nullptr");
		return;
	}

	FString address;
	if (!m_sessionInterface->GetResolvedConnectString(_sessionName, address))
	{
		V_LOG("Could not ge connect string!!!");
		return;
	}

	UEngine* engine = GetEngine();
	if (engine == nullptr) { return; }

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr))
	{
		engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("PlayerController Not Find")));
		return;
	}
	playerController->ClientTravel(FString(*address), ETravelType::TRAVEL_Absolute);
}

//=====================================================================================

void UTAInstance::CreateSession()
{
	if (m_sessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bIsLANMatch = true;
		sessionSettings.NumPublicConnections = 10;
		sessionSettings.bShouldAdvertise = true;
		m_sessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

void UTAInstance::TAServer()
{
	V_LOG(FString("TAHost......"));

	UEngine* engine = GetEngine();
	if (engine == nullptr) { return; }

	//engine->AddOnScreenDebugMessage(0, 5, FColor::Red, TEXT("Host....."));

	UWorld* world = GetWorld();
	if (world == nullptr) { return; }

	world->ServerTravel("/Game/TA/Global/Maps/TAServer?listen");
	
	if (m_mainMenuInstance != nullptr)
	{
		m_mainMenuInstance->TearDown();
	}
}

void UTAInstance::TAJoin(const FString& _address)
{
	UEngine* engine = GetEngine();
	if (engine == nullptr) { return; }
	engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("Hi You Just Joining %s"),*_address));

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr))
	{
		engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("PlayerController Not Find")));
		return;
	}
	playerController->ClientTravel(FString(*_address), ETravelType::TRAVEL_Absolute);

	if (m_mainMenuInstance != nullptr)
	{
		m_mainMenuInstance->TearDown();
	}

}

void UTAInstance::TAJoinVisin()
{
	UEngine* engine = GetEngine();
	if (engine == nullptr) { return; }

	engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("Find Engine")));

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr))
	{
		engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("Could Not Find PlayerController")));
		return;
	}

	playerController->ClientTravel(FString("192.168.14.22"),ETravelType::TRAVEL_Absolute);
	
}

void UTAInstance::TAFindFirstLocalPlayer()
{
	UEngine* engine = GetEngine();
	if (engine == nullptr) { return; }

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (playerController == nullptr)
	{
		engine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("PlayerController Not Find")));
	}
	else
	{
		engine->AddOnScreenDebugMessage(0, 5, FColor::Red, FString::Printf(TEXT("FindLocal Player %s"),*(playerController->GetName())));
	}
}

void UTAInstance::TALoadMainMenu()
{
	if (!ensure(m_MainMenuClass != nullptr)) {
		V_LOG(FString("m_MainMenuClass is Null"));
		return;
	}

	m_mainMenuInstance = CreateWidget<UMainMenu>(this, m_MainMenuClass);

	if (!ensure(m_mainMenuInstance != nullptr))
	{
		V_LOG(FString("m_MainMenuInstance is Null"));
		return;
	}

	m_mainMenuInstance->SetUp(this);
}

void UTAInstance::TALoadInGameMenu()
{
	if (!ensure(m_InGameMenuClass != nullptr)) {
		V_LOG(FString("m_InGameMenuClass is Null"));
		return;
	}

	UMenuWidget* inGameMenu = CreateWidget<UMenuWidget>(this, m_InGameMenuClass);

	if (!ensure(inGameMenu != nullptr))
	{
		V_LOG(FString("inGameMenu is Null"));
		return;
	}
	inGameMenu->SetUp(this);
}


