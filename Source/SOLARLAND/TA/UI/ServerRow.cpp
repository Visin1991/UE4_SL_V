// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UServerRow::Setup(class UMainMenu* _parent, uint32 _index)
{
	parent = _parent;
	Index = _index;
	RowButton->OnClicked.AddDynamic(this,&UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UServerRow OnClicked"));

	parent->SelectedIndexFunc(Index);
	UE_LOG(LogTemp,Warning,TEXT("Selected Index : %d"),Index);
}