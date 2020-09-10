// Fill out your copyright notice in the Description page of Project Settings.


#include "NetCarComponent.h"
#include "DrawDebugHelpers.h"
#include "CarsGameInstance.h"
#include "Game/Car.h"
#include "Game/Projectile.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameNet/GameBuffer.h"
#include "Net/Manager.h"
#include "Game/CarMovementComponent.h"

namespace {
	const float s_fSnapshotDelay = 1.f;
	const float s_fMaxError = 100.f;
}

// Sets default values for this component's properties
UNetCarComponent::UNetCarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNetCarComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pManager = Net::CManager::getSingletonPtr();

}



void UNetCarComponent::SetInput(FVector2D _vInput)
{
	if (m_vMovementInput != _vInput) {
		m_vMovementInput = _vInput;
		sendCommand = true;
	}
}

void UNetCarComponent::SetShootInput(bool isShootPressed)
{
	if (isShootPressed == true) {
		sendShootCommand = true;
	}
}

// Called every frame
void UNetCarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UCarsGameInstance* pGI = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());

	ACar* pCar = pGI->GetGameNetMgr()->GetOwnCar();

	bool bOwner = pCar == GetOwner();
	if (m_pManager->getID() == Net::ID::SERVER || bOwner)
	{
		timeToNextSnapshot -= DeltaTime;
		SerializeData();
	}
	if (m_pManager->getID() != Net::ID::SERVER)
	{
		if (bOwner) {
			SimulateOwnCarMovement(DeltaTime);
		}
		else {
			SimulateCarMovement(DeltaTime);
		}
	}
}

void UNetCarComponent::SerializeData()
{
	if (m_pManager->getID() == Net::ID::SERVER)
	{
		if (timeToNextSnapshot <= 0) {
			timeToNextSnapshot += s_fSnapshotDelay;
			CGameBuffer oData;
			Net::NetMessageType xType = Net::ENTITY_MSG;
			oData.write(xType);
			UCarsGameInstance* pGameInstance = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());
			ACar* pCar = Cast<ACar>(GetOwner());
			check(pCar);
			oData.write(pGameInstance->GetGameNetMgr()->GetCarID(pCar));
			oData.write(GetOwner()->GetActorTransform());

			float fVelocity = pCar->GetCarMovement()->GetVelocityMagnitude();
			oData.write(fVelocity);

			m_pManager->send(oData.getbuffer(), oData.getSize(), true);
		}
	}
	else // client
	{
		if (sendCommand) {
			sendCommand = false;
			CGameBuffer oData;
			Net::NetMessageType xType = Net::ENTITY_MSG;
			oData.write(xType);
			oData.write(m_pManager->getID());
			oData.write(m_vMovementInput);
			m_pManager->send(oData.getbuffer(), oData.getSize(), true);
		}
		if (sendShootCommand) {
			sendShootCommand = false;
			CGameBuffer oData;
			Net::NetMessageType xType = Net::CREATE_ENTITY;
			oData.write(xType);
			oData.write(m_pManager->getID());
			m_pManager->send(oData.getbuffer(), oData.getSize(), true);
		}
	}
}


void UNetCarComponent::DeserializeData(CGameBuffer& _rData)
{
	if (m_pManager->getID() == Net::ID::SERVER)
	{
		FVector2D vInput;
		_rData.read(vInput);
		ACar* pCar = Cast<ACar>(GetOwner());
		pCar->GetCarMovement()->SetInput(vInput);
	}
	else
	{
		FTransform tTransform;
		_rData.read(tTransform);
		float fVelocity;
		_rData.read(fVelocity);
		vVelocity = tTransform.GetRotation().GetAxisX() * fVelocity;

		UCarsGameInstance* pGI = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());

		ACar* pCar = pGI->GetGameNetMgr()->GetOwnCar();

		bool bOwner = pCar == GetOwner();
		if (fVelocity > 0.0f && bOwner) {
			FRotator newRotation = FRotationMatrix::MakeFromX(vVelocity).Rotator();
			GetOwner()->SetActorRotation(newRotation);
		}
		FVector vDesiredPos = tTransform.GetLocation();
		FVector vCurrentPos = GetOwner()->GetActorLocation();
		vError = vDesiredPos - vCurrentPos;

		if (vError.SizeSquared() < s_fMaxError * s_fMaxError) {
			FVector vTargetPos = vDesiredPos + vVelocity * s_fSnapshotDelay;
			vVelocity = (vTargetPos - vCurrentPos) / s_fSnapshotDelay;
		}
		else {
			GetOwner()->SetActorTransform(tTransform);
		}


	}
}

void UNetCarComponent::SimulateCarMovement(float DeltaTime) {
	ACar* pCar = Cast<ACar>(GetOwner());
	pCar->GetCarMovement()->MoveActor(vVelocity, DeltaTime);
}

void UNetCarComponent::SimulateOwnCarMovement(float DeltaTime) {
	ACar* pCar = Cast<ACar>(GetOwner());
	pCar->GetCarMovement()->SetInput(m_vMovementInput);
	//original Transform
	FTransform oTransform = GetOwner()->GetTransform();
	FVector vReduction = vError * DeltaTime;
	if (vReduction.SizeSquared() > vError.SizeSquared()) {
		vReduction = vError;
	}
	vError -= vReduction;
	oTransform.AddToTranslation(vReduction);
	GetOwner()->SetActorTransform(oTransform);
}

void UNetCarComponent::SetInputToZero() {
	SetInput(FVector2D::ZeroVector);
}