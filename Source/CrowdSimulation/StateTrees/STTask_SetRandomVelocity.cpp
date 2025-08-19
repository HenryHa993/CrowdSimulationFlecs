#include "STTask_SetRandomVelocity.h"
#include "GameFramework/FloatingPawnMovement.h"

EStateTreeRunStatus FSTTask_SetRandomVelocity::EnterState(FStateTreeExecutionContext& Context,
                                                          const FStateTreeTransitionResult& Transition) const
{
	FSetRandomVelocityTaskInstanceData& data = Context.GetInstanceData(*this);
	
	if(!data.Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner pawn not set"));
		return EStateTreeRunStatus::Failed;
	}

	UPawnMovementComponent* movementComp = data.Actor->GetMovementComponent();
	if(!movementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement component not set"));
		return EStateTreeRunStatus::Failed;
	}
	FVector2D randUnitVector = FMath::RandPointInCircle(1);
	FVector randVelocity = FVector(randUnitVector.X, randUnitVector.Y, 0)  * data.Speed;
	movementComp->Velocity = randVelocity;
	FQuat correctionQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(-90.0f));
	data.Actor->SetActorRotation((randVelocity.ToOrientationQuat() * correctionQuat).GetNormalized());
	return EStateTreeRunStatus::Succeeded;
}

EStateTreeRunStatus FSTTask_SetRandomVelocity::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FSetRandomVelocityTaskInstanceData& data = Context.GetInstanceData(*this);
	UE_LOG(LogTemp, Warning, TEXT("Rotating actor"));
	FQuat correctionQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(-90.0f));
	data.Actor->SetActorRotation(FQuat::FastLerp(data.Actor->GetActorRotation().Quaternion(), data.RandomVelocity.ToOrientationQuat() * correctionQuat, DeltaTime).GetNormalized());
	return EStateTreeRunStatus::Succeeded;
}
