// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "GameNet/NetProjectileComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    if (m_pMesh != nullptr) {
        m_pMesh->SetupAttachment(RootComponent);
        static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
        if (BoxVisualAsset.Succeeded())
        {
            m_pMesh->SetStaticMesh(BoxVisualAsset.Object);
            static ConstructorHelpers::FObjectFinder<UMaterial> ProjectileMaterial(TEXT("Material'/Game/Textures/Projectile_mat'"));
            m_pMesh->SetMaterial(0, ProjectileMaterial.Object);
        }
        SetActorScale3D(FVector(0.1f, 0.1f, 0.05f));
    }   
    // Puede petar porque el proyectil no tiene que tener un componente de red? EFECTIVAMENTE
    m_pNet = CreateDefaultSubobject<UNetProjectileComponent>(TEXT("NetProjectileComponent"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //CheckCollisionWithCar();

    timeLasts += DeltaTime;
    if (timeLasts >= lifeTime) {
        Destroy();
    }

    MoveActor(DeltaTime);
}


void AProjectile::MoveActor(float DeltaTime) {
    auto oTrans = GetActorTransform();
    oTrans.AddToTranslation((GetActorForwardVector() * DeltaTime) * velocityFactor);
    SetActorTransform(oTrans);
    if (GetActorForwardVector() != FVector::ZeroVector)
    {
        FRotator Rot = FRotationMatrix::MakeFromX(GetActorForwardVector() * velocityFactor).Rotator();
        SetActorRotation(Rot);
    }
}