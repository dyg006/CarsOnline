
#include "FinishLine.h"
// include draw debu helpers header file
#include "DrawDebugHelpers.h"
#include "CheckPoint.h"
#include "Engine/World.h"

AFinishLine::AFinishLine()
{
  //Register Events
  OnActorBeginOverlap.AddDynamic(this, &AFinishLine::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
  Super::BeginPlay();
  for (ACheckPoint* pCP : m_vCheckPoints)
  {
    pCP->SetFinishLine(this);
  }
  DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, FLT_MAX, 0, 5);
}

void AFinishLine::AddPassedCheckPoint(ACheckPoint* _pCheckPoint)
{
  if (!m_vPassedCheckPoints.Contains(_pCheckPoint))
  {
    m_vPassedCheckPoints.Add(_pCheckPoint);
  }
}
void AFinishLine::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
  // check if Actors do not equal nullptr and that
  if (OtherActor && (OtherActor != this))
  {
    float fCurrentTime = GetWorld()->GetTimeSeconds();
    if (m_vPassedCheckPoints.Num() == m_vCheckPoints.Num())
    {
      m_fLastLapTime = fCurrentTime - m_fPreviousTime;
      m_fPreviousTime = fCurrentTime;
      if (m_fBestTime > m_fLastLapTime)
      {
        m_fBestTime = m_fLastLapTime;
        // Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString("Best Lap!"));
      }
      ++m_uLaps;
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString("Lap " + FString::FromInt(m_uLaps) + "(" + FString::SanitizeFloat(m_fLastLapTime) + " seconds)"));
    }
    else
    {
      /*if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *FString("Invalid lap!"));
      }*/
    }
    m_vPassedCheckPoints.Reset();
  }
}
