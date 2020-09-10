
#include "CheckPoint.h"
#include "DrawDebugHelpers.h"
#include "FinishLine.h"

ACheckPoint::ACheckPoint()
{
  //Register Events
  OnActorBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
  Super::BeginPlay();
  DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, FLT_MAX, 0, 5);
}

void ACheckPoint::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
  // check if Actors do not equal nullptr and that
  if (OtherActor && (OtherActor != this) && m_pFinishLine)
  {
    m_pFinishLine->AddPassedCheckPoint(this);
  }
}
