#include "STTask_SetISMAnimation.h"

#include "CrowdSimulation/Pawns/ISMUnit.h"

EStateTreeRunStatus FSTTask_SetISMAnimation::EnterState(FStateTreeExecutionContext& Context,
                                                        const FStateTreeTransitionResult& Transition) const
{
	FSetISMAnimationInstanceData& data = Context.GetInstanceData(*this);
	if (!data.Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimationTask: Actor is not set"));
		return EStateTreeRunStatus::Failed;
	}
	AISMUnit* ismUnit = Cast<AISMUnit>(data.Actor);
	if(!ismUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimationTask: Could not cast to ISMUnit"));
		return EStateTreeRunStatus::Failed;
	}
	if(!ismUnit->ISMController)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimationTask: ISMController not set"));
		return EStateTreeRunStatus::Failed;
	}
	ismUnit->ISMController->SetAnimation(ismUnit->Index, data.StartFrame, data.EndFrame);
	return EStateTreeRunStatus::Succeeded;
}
