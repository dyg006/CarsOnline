// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Car.generated.h"

class UCarMovementComponent;
class UNetCarComponent;

UCLASS()
class CARS_API ACar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();
  // Called every frame
  virtual void Tick(float DeltaTime) override;
  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
  float GetVelocityMagnitude();
  void SetInput(const FVector2D& _vInput, bool isShootPressed) { m_vMovementInput = _vInput; shootedPressed = isShootPressed; }
  UNetCarComponent* GetNetComponent() { return m_pNet; }
  UCarMovementComponent* GetCarMovement() { return m_pCarMovement; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  //Input functions
  void Move(float AxisValue);
  void Turn(float AxisValue);
  void Shoot();
  void ShootReleased();

  /// <summary>
  /// Notifies the car when an actor begin overlapping with it.
  /// </summary>
  /// <param name="OtherActor">The actor that causes the overlap.</param>
  void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
  //Mesh
  UPROPERTY(EditAnywhere)
  UStaticMeshComponent* m_pMesh;
  //Movement
  UPROPERTY(EditAnywhere)
  UCarMovementComponent* m_pCarMovement;
  // Net
  UPROPERTY(EditAnywhere)
  UNetCarComponent* m_pNet;
  //Input variables
  FVector2D m_vMovementInput = FVector2D::ZeroVector;
  bool shootedPressed = false;
};
