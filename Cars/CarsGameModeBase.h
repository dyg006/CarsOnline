// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CarsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CARS_API ACarsGameModeBase : public AGameModeBase
{
  GENERATED_BODY()
public:
  ACarsGameModeBase(const class FObjectInitializer& ObjectInitializer);
  virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	
  virtual void Tick(float DeltaTime) override;
	
};
