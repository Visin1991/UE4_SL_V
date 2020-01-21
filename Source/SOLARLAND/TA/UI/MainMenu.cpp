// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "../Global/TAStatic.h"
#include "Engine/Engine.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> bp_ServerRowClass(TEXT("/Game/TA/Global/UI/WBP_ServerRow"));
	if (!ensure(bp_ServerRowClass.Class != nullptr))
	{
		UE_LOG(LogTemp,Warning,TEXT("/Game/TA/Global/UI/WBP_ServerRow not exist"));
		return;
	}
	m_ServerRowClass = bp_ServerRowClass.Class;

}

bool UMainMenu::Initialize()
{
	bool  sucess = Super::Initialize();
	if (!sucess) return false;

	if (!ensure(ServerButton != nullptr))return false;
	if (!ensure(ClientButton != nullptr))return false;
	

	//TODO: setup
	ServerButton->OnClicked.AddDynamic(this,&UMainMenu::StartServer);
	ClientButton->OnClicked.AddDynamic(this, &UMainMenu::OpenConnectionMenu);
	BackToMain->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::StartServer()
{
	V_LOG(FString("Host Server Button Called......"));
	if (m_menuInterface != nullptr)
	{
		m_menuInterface->Server();
	}
}

void UMainMenu::SetServerList(TArray<FString> _serverName)
{
	UE_LOG(LogTemp,Warning,TEXT("Start Set ServerList"));

	UWorld* world = this->GetWorld();
	if (!ensure(world != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("World Not Exist"));
		return;
	}

	if (ServerList == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server List is nullptr"));
		return;
	}

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FString& serverName : _serverName)
	{
		auto serverRow = CreateWidget<UServerRow>(world, m_ServerRowClass);

		if (!ensure(serverRow != nullptr)) {
			UE_LOG(LogTemp, Warning, TEXT("Cannot create Widget UServerRow"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Add Server %s"),*serverName);

		serverRow->ServerName->SetText(FText::FromString(serverName));
		serverRow->Setup(this,i);
		++i;
		ServerList->AddChild(serverRow);
	}

}

void UMainMenu::SelectedIndexFunc(uint32 _index)
{
	SelectedIndex = _index;
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp,Warning,TEXT("Join Server......"));

	if (!SelectedIndex.IsSet()) { V_LOG("SelecetdIndex Not Set"); return; }

	if (m_menuInterface == nullptr) { V_LOG("m_Interface is nullptr"); return; }

	//m_menuInterface->RefreshServerList();
}

void UMainMenu::OpenConnectionMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenConnectionMenu"));
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(ConnectionMenu != nullptr))return;
	MenuSwitcher->SetActiveWidget(ConnectionMenu);
	if (m_menuInterface == nullptr) { UE_LOG(LogTemp,Warning,TEXT("Menu Interface is NULL"))return; }
	m_menuInterface->RefreshServerList();
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(MainMenu != nullptr))return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}
