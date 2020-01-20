// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SOLARLAND_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void StartServer();

	UFUNCTION()
	void OpenConnectionMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void JoinServer();


protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ClientButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackToMain;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinServerButton;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* ConnectionMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressField;
};
