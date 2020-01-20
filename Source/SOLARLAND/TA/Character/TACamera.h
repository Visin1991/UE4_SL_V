// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "TACamera.generated.h"

class ATAPlayerController;
/**
 * 
 */
UCLASS()
class SOLARLAND_API ATACamera : public ACameraActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ATAPlayerController* m_PlayerController;
};
