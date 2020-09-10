// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameNet/NetComponent.h"
#include "GameNet/NetCarComponent.h"
#include "NetProjectileComponent.generated.h"

class ACar;
class AProjectile;

/**
 * 
 */
UCLASS()
class CARS_API UNetProjectileComponent : public UNetComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CheckProjectileCollision();
	void SimulateCarMovement(float DeltaTime);
	void SimulateOwnCarMovement(float DeltaTime);

public:
	// Sets default values for this component's properties
	UNetProjectileComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SerializeData(ACar* carHit);

	void ReplicateProjectCollision(ACar* carHit);

private:
	Net::CManager* m_pManager;
	float timeToNextSnapshot = 0.f;

	class ACar* ownCar;

	const float distanceToCollide = 25.f;

	bool projectileDestroyed = false;

	TArray<AActor*> currentCars;

	AProjectile* projectile;

	
};
