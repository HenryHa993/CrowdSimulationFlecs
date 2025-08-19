#include "STTask_SetVelocity.h"
#include "GameFramework/FloatingPawnMovement.h"

EStateTreeRunStatus FSTTask_SetVelocity::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FSetVelocityInstanceData& data = Context.GetInstanceData(*this);
	if (!data.Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor is not set"));
		return EStateTreeRunStatus::Failed;
	}
	UPawnMovementComponent* movementComp = data.Actor->GetMovementComponent();
	if (!movementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement component not found"));
		return EStateTreeRunStatus::Failed;
	}
	movementComp->Velocity = data.Velocity;
	return EStateTreeRunStatus::Succeeded;
}
