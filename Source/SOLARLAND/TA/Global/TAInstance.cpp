// Fill out your copyright notice in the Description page of Project Settings.


#include "TAInstance.h"
#include "TAStatic.h"
#include "GameFramework/PlayerController.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../UI/MainMenu.h"
#include "../UI/MenuWidget.h"


/*
	1. Load The main menu UI Blueprint

	2. TAMain map Level Blue print----> Get TAInstance  ---> Call TALoadMainMenu()
		Inject TAInstance -->To--> MainMenu
				MainMenu::Server()-->Call--->TAInstance::Server()
		
*/


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
	
}

void UTAInstance::TAServer()
{
	V_LOG(FString("TAHost......"));

	UEngine* engine = GetEngine();
	if (engine == nullptr) { return; }

	//engine->AddOnScreenDebugMessage(0, 5, FColor::Red, TEXT("Host....."));

	UWorld* world = GetWorld();
	if (world == nullptr) { return; }

	world->ServerTravel("/Game/TA/Ref/AbandonedFactory/Maps/Warehouse_01_day/Main_Warehouse_01?listen");
	
	if (m_mainMenuInstance != nullptr)
	{
		m_mainMenuInstance->TearDown();
	}
}

void UTAInstance::Server()
{
	TAServer();
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

void UTAInstance::JoinServer(const FString& _address)
{
	TAJoin(_address);
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


