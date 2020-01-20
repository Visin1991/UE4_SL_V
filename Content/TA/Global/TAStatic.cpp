// Fill out your copyright notice in the Description page of Project Settings.


#include "TAStatic.h"

void UTAStatic::TALogWarning(FString info)
{
	V_LOG(info);
}

FVector UTAStatic::TAMakeVector(float x, float y, float z)
{
	return FVector(x, y, z);
}
