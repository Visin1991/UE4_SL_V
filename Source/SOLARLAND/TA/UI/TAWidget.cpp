// Fill out your copyright notice in the Description page of Project Settings.


#include "TAWidget.h"
#include "../Global/TAInstance.h"


bool UTAWidget::Initialize()
{
	Super::Initialize();

	return true;
}

void UTAWidget::TA_RefreshServerList()
{
	auto instance = Cast<UTAInstance>(GetGameInstance());
	if (instance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Instance is not UTAInstance!!!!"));
		return;
	}

	if(!instance->OnFindSessionsCompleteEvent.IsBoundToObject(this))
	{
		OnFindSessionsCompleteHandle = instance->OnFindSessionsCompleteEvent.AddUObject(this, &UTAWidget::OnFindSessionsComplete_SetServerNames);
	}

	instance->RefreshServerList();
}

void UTAWidget::OnFindSessionsComplete_SetServerNames(TArray<FString> _names)
{
	m_ServerNames = _names;
	TA_OnFindSessionsComplete_BP();
}

TArray<FString> UTAWidget::GetServerNames()
{
	return m_ServerNames;
}