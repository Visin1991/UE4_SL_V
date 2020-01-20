// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "../Global/TAStatic.h"

void UMenuWidget::SetUp(IMenuInterface* _mainMenuInterface)
{
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr))return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr))
	{
		V_LOG_ERROR(FString("Could Not Find PlayerController"));
		return;
	}

	FInputModeUIOnly inputModeData;
	inputModeData.SetWidgetToFocus(this->TakeWidget());
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(inputModeData);
	playerController->bShowMouseCursor = true;

	m_menuInterface = _mainMenuInterface;
}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr))return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr))
	{
		V_LOG_ERROR(FString("Could Not Find PlayerController"));
		return;
	}

	FInputModeGameOnly InputModeData;
	playerController->SetInputMode(InputModeData);
	playerController->bShowMouseCursor = false;
}

