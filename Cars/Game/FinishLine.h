#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "FinishLine.generated.h"

class ACheckPoint;

UCLASS()
class CARS_API AFinishLine : public ATriggerBox
{
  GENERATED_BODY()

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // constructor sets default values for this actor's properties
  AFinishLine();
  // overlap begin function
  UFUNCTION()
  void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
  void AddPassedCheckPoint(ACheckPoint* _pCheckPoint);

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ACheckPoint*> m_vCheckPoints;

  TArray<ACheckPoint*> m_vPassedCheckPoints;
  unsigned int m_uLaps = 0u;
  float m_fPreviousTime = 0.f;
  float m_fLastLapTime = 0.f;
  float m_fBestTime = FLT_MAX;
};