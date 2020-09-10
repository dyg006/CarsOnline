// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARS_API UCarMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCarMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
  // Movement Calculation
  FVector CalculateAcceleration() const;
  FVector CalculateAverageVelocity(const FVector& _vAccel, float DeltaTime);
  void ClampVelocity(FVector& _vVelocity) const;
  
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  // Velocity magnitude
  float GetVelocityMagnitude() { return m_vVelocity.Size(); }
  // Input
  void SetInput(FVector2D _vInput) { if (!inputBlocked) { m_vMovementInput = _vInput; } }
  void MoveActor(const FVector& _vVelocity, float DeltaTime);

  bool inputBlocked = false;

  void StopMovementImmediately();

protected:
  // Constants
  UPROPERTY(EditAnywhere)
    float m_fAccel = 120.f; // Forward accel
  UPROPERTY(EditAnywhere)
    float m_fDrag = 50.f; // Drag accel (no input)
  UPROPERTY(EditAnywhere)
    float m_fBrakeAccel = 300.f; // Brake accel
  UPROPERTY(EditAnywhere)
    float m_fMaxVelocity = 250.f; // Max Velocity
  UPROPERTY(EditAnywhere)
    float m_fRotationFactor = 2.f; // For side accels
    //Current velocity
  FVector m_vVelocity = FVector::ZeroVector;
  //Input vector
  FVector2D m_vMovementInput = FVector2D::ZeroVector;
		
};
