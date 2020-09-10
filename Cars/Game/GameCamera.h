// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameCamera.generated.h"

class ACar;

UCLASS()
class CARS_API AGameCamera : public ACameraActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AGameCamera();
  // Called every frame
  virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  void SetTarget();
private:
  UPROPERTY(EditAnywhere)
  ACar* m_pTarget;
  UPROPERTY(EditAnywhere)
  float m_fMinDistance = 200.f;
  UPROPERTY(EditAnywhere)
  float m_fDistanceFromVelocityFactor = 0.6f;
};
