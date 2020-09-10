// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UCarMovementComponent::UCarMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCarMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  FVector vAccel = CalculateAcceleration();
  FVector vAverageVelocity = CalculateAverageVelocity(vAccel, DeltaTime);
  MoveActor(vAverageVelocity, DeltaTime);
}

FVector UCarMovementComponent::CalculateAcceleration() const
{
  FVector v_Accel;
  if (m_vMovementInput.Y > 0.f)
  {
    v_Accel = m_fAccel * m_vMovementInput.Y * GetOwner()->GetActorForwardVector();
  }
  else if (m_vMovementInput.Y == 0.f)
  {
    v_Accel = -m_fDrag * GetOwner()->GetActorForwardVector();
  }
  else
  {
    v_Accel = m_fBrakeAccel * m_vMovementInput.Y * GetOwner()->GetActorForwardVector();
  }
  v_Accel += m_vMovementInput.X * m_vVelocity.Size() * m_fRotationFactor * GetOwner()->GetActorRightVector();

  return v_Accel;
}

FVector UCarMovementComponent::CalculateAverageVelocity(const FVector& _vAccel, float DeltaTime)
{
  FVector vAverageVelocity = m_vVelocity + 0.5f * _vAccel * DeltaTime;
  ClampVelocity(vAverageVelocity);
  // Updating velocity for the next step
  m_vVelocity += _vAccel * DeltaTime;
  ClampVelocity(m_vVelocity);
  return m_vVelocity;
}

void UCarMovementComponent::ClampVelocity(FVector& _vVelocity_) const
{
  if (_vVelocity_ != FVector::ZeroVector &&
    FVector::DotProduct(_vVelocity_, GetOwner()->GetActorForwardVector()) < 0.f)
  {
    _vVelocity_ = FVector::ZeroVector;
  }
  else if (_vVelocity_.Size() > m_fMaxVelocity)
  {
    _vVelocity_ *= m_fMaxVelocity / _vVelocity_.Size();
  }
}

void UCarMovementComponent::MoveActor(const FVector& _vAverageVelocity, float DeltaTime)
{
  auto oTrans = GetOwner()->GetActorTransform();
  oTrans.AddToTranslation(_vAverageVelocity * DeltaTime);
  FVector PlayerLoc = GetOwner()->GetActorLocation();
  GetOwner()->SetActorTransform(oTrans);
  if (_vAverageVelocity != FVector::ZeroVector)
  {
    FRotator Rot = FRotationMatrix::MakeFromX(_vAverageVelocity).Rotator();
    GetOwner()->SetActorRotation(Rot);
  }
}

void UCarMovementComponent::StopMovementImmediately() {
    m_vVelocity = FVector::ZeroVector;
}