// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameNet/NetComponent.h"
#include "NetCarComponent.generated.h"



/**
 * 
 */
UCLASS()
class CARS_API UNetCarComponent : public UNetComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNetCarComponent();

	void SetInput(FVector2D _vInput);
	void SetInputToZero();
	void SetShootInput(bool isShootPressed);

	void SerializeData();
	void DeserializeData(CGameBuffer& _rData);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SimulateCarMovement(float DeltaTime);
	void SimulateOwnCarMovement(float DeltaTime);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector2D m_vMovementInput;
	bool shootPressed = false;
	Net::CManager* m_pManager;
	bool sendCommand = true;
	bool sendShootCommand = false;
	float timeToNextSnapshot = 0.f;
	FVector vVelocity = FVector::ZeroVector;
	FVector vError = FVector::ZeroVector;
};
