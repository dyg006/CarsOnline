// Fill out your copyright notice in the Description page of Project Settings.


#include "NetProjectileComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Car.h"
#include "Game/CarMovementComponent.h"
#include "Game/Projectile.h"
#include "CarsGameInstance.h"
#include "Net/Manager.h"
#include "Engine/World.h"

namespace {
	const float s_fSnapshotDelay = 1.f;
	const float s_fMaxError = 100.f;
}

// Sets default values for this component's properties
UNetProjectileComponent::UNetProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UNetProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pManager = Net::CManager::getSingletonPtr();

	TSubclassOf<ACar> classToFind = ACar::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, currentCars);

	projectile = Cast<AProjectile>(GetOwner());
}


void UNetProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Comprobamos que estamos en el servidor y que el proyectil no ha sido disparado por nosotros para comprobar la colision del mismo
	if (m_pManager->getID() == Net::ID::SERVER && !projectileDestroyed) {
		CheckProjectileCollision();
	}
}

void UNetProjectileComponent::CheckProjectileCollision() {

	for (AActor* car : currentCars) {
		if (car != projectile->ownCar) {
			//Check if projectile is close to a car
			if (car->GetDistanceTo(GetOwner()) < distanceToCollide) {
				/*if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "projectile has collided");
				}*/
				//if a projectile collides with this car we reduce the car velocity to zero and destroy the projectile
				Cast<ACar>(car)->GetNetComponent()->SetInputToZero();
				Cast<ACar>(car)->GetCarMovement()->StopMovementImmediately();

				SerializeData(Cast<ACar>(car));
				UCarsGameInstance* pGI = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());

				GetOwner()->Destroy();

				projectileDestroyed = true;
				break;
			}
		}
	}
}


void UNetProjectileComponent::SerializeData(ACar* carHit) {
	UCarsGameInstance* pGI = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());

	CGameBuffer oData;
	Net::NetMessageType xType = Net::DESTROY_PROJECTILE;
	oData.write(xType);
	oData.write(pGI->GetGameNetMgr()->GetProjectileID(projectile));
	oData.write(pGI->GetGameNetMgr()->GetCarID(carHit));
	m_pManager->send(oData.getbuffer(), oData.getSize(), true);
}

void UNetProjectileComponent::ReplicateProjectCollision(ACar* carHit)
{
	if (m_pManager->getID() != Net::ID::SERVER)
	{
		UCarsGameInstance* pGI = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());

		ACar* ownCar = pGI->GetGameNetMgr()->GetOwnCar();
		if (ownCar == carHit) {
			ownCar->GetNetComponent()->SetInputToZero();
			ownCar->GetCarMovement()->StopMovementImmediately();
		}
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Projectile destroyed on client");
		}*/
		GetOwner()->Destroy();
	}
}

