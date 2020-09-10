// Fill out your copyright notice in the Description page of Project Settings.

#include "CarsPlayerController.h"


ACarsPlayerController::ACarsPlayerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
  bAutoManageActiveCameraTarget = false;
}

void ACarsPlayerController::BeginPlay()
{
  Super::BeginPlay();
  SetInputMode(FInputModeGameAndUI());
}
