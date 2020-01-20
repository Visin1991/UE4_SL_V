// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../Global/TAStatic.h"
#include "Engine/Engine.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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

void UMainMenu::JoinServer()
{
	if (m_menuInterface != nullptr)
	{
		if (!ensure(IPAddressField != nullptr)) return;
		const FString Address = IPAddressField->GetText().ToString();
		m_menuInterface->JoinServer(Address);
	}
}

void UMainMenu::OpenConnectionMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(ConnectionMenu != nullptr))return;
	MenuSwitcher->SetActiveWidget(ConnectionMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(MainMenu != nullptr))return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}
