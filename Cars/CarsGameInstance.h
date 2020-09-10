// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameNet/GameNetMgr.h"
#include "CarsGameInstance.generated.h"

class UUserWidget;
namespace Net
{
  class CManager;
}
/**
 * 
 */
UCLASS()
class CARS_API UCarsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
  UCarsGameInstance(const FObjectInitializer& _oi);
  virtual ~UCarsGameInstance();

  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void OnServerButtonClick(FString sPort);
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void OnClientButtonClick(FString sIP, FString sPort);
  UFUNCTION(BlueprintCallable, Category = CarsNet)
  void OnServerStartButtonClick();

  const CGameNetMgr* GetGameNetMgr() const { return &m_oGameNetMgr; }

protected:
  virtual void OnStart() override;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CarsNet)
  TSubclassOf<UUserWidget> StartingWidgetClass;
  UPROPERTY()
  UUserWidget* CurrentWidget;
  CGameNetMgr m_oGameNetMgr;
  Net::CManager* m_pManager = nullptr;
	
};
