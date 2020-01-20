// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "../Global/TAStatic.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool  sucess = Super::Initialize();
	if (!sucess) return false;

	if (!ensure(CancelButton != nullptr))return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	if (!ensure(QuitButton != nullptr))return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}

void UInGameMenu::CancelPressed()
{
	TearDown();
}

void UInGameMenu::QuitPressed()
{
	TearDown();

	if (m_menuInterface != nullptr)
	{
		V_LOG(FString("TearDown......"));
		m_menuInterface->LoadMainMenu();
	}
}