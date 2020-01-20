// Fill out your copyright notice in the Description page of Project Settings.


#include "TAGameMode.h"
#include "UObject/ConstructorHelpers.h"

ATAGameMode::ATAGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> playerBPClass(TEXT("/Game/TA/Global/Blueprints/BP_TACharacter"));
	if (playerBPClass.Class != nullptr)
	{
		DefaultPawnClass = playerBPClass.Class;
	}
}

