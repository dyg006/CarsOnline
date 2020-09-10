// Fill out your copyright notice in the Description page of Project Settings.

#include "CarsGameModeBase.h"
#include "Game/CarsPlayerController.h"
#include "Net/Manager.h"

ACarsGameModeBase::ACarsGameModeBase(const class FObjectInitializer& ObjectInitializer) : AGameModeBase(ObjectInitializer)
{
  PlayerControllerClass = ACarsPlayerController::StaticClass();
  PrimaryActorTick.bCanEverTick = true;
}

APawn* ACarsGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
  return nullptr;
}

void ACarsGameModeBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  Net::CManager* pManager = Net::CManager::getSingletonPtr();
  if (pManager)
  {
    pManager->tick();
  }
}
