// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UCar;
class UNetProjectileComponent;

UCLASS()
class CARS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UNetProjectileComponent* GetNetComponent() { return m_pNet; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_pMesh;

	UPROPERTY(EditAnywhere)
	UNetProjectileComponent* m_pNet;

	const float velocityFactor = 200.f;
	const float lifeTime = 5.f;
	float timeLasts = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveActor(float DeltaTime);

	//Reference to the car that has created the projectile
	class ACar* ownCar;
};
