// Fill out your copyright notice in the Description page of Project Settings.


#include "CarsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameNet/GameBuffer.h"
#include "Kismet/GameplayStatics.h"

UCarsGameInstance::UCarsGameInstance(const FObjectInitializer& _oi)
  : Super(_oi)
  , m_oGameNetMgr(this)
{
  m_pManager = Net::CManager::getSingletonPtr();
}

UCarsGameInstance::~UCarsGameInstance()
{

}

void UCarsGameInstance::OnStart()
{
  Super::OnStart();
  ChangeMenuWidget(StartingWidgetClass);
}

void UCarsGameInstance::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
  if (CurrentWidget)
  {
    CurrentWidget->RemoveFromViewport();
    CurrentWidget = nullptr;
  }
  if (NewWidgetClass != nullptr)
  {
    CurrentWidget = CreateWidget<UUserWidget>(this, NewWidgetClass);
    if (CurrentWidget)
    {
      CurrentWidget->AddToViewport();
    }
  }
}

void UCarsGameInstance::OnServerButtonClick(FString sPort)
{
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *FString("Server"));
  m_pManager->activateAsServer(FCString::Atoi(*sPort));
}

void UCarsGameInstance::OnClientButtonClick(FString sIP, FString sPort)
{
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *FString("Client"));
  m_pManager->activateAsClient();
  m_pManager->connectTo(TCHAR_TO_ANSI(*sIP), FCString::Atoi(*sPort));
}

void UCarsGameInstance::OnServerStartButtonClick()
{
  // Cargar nivel en el servidor
  char* sLevel = "Circuit1";
  UGameplayStatics::OpenLevel(GetWorld(), sLevel);
  // mandar paquete con el mapa a cargar
  CGameBuffer data;
  Net::NetMessageType iID = Net::LOAD_MAP;
  data.write(iID);
  data.write(sLevel);
  m_pManager->send(data.getbuffer(), data.getSize(), true);
}
