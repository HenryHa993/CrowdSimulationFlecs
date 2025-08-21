#include "STTask_SetTSAnimation.h"

#include "TurboSequence_Manager_Lf.h"
#include "CrowdSimulation/Pawns/TSUnit.h"

EStateTreeRunStatus FSTTask_SetTSAnimation::EnterState(FStateTreeExecutionContext& Context,
                                                       const FStateTreeTransitionResult& Transition) const
{
	FSetTSAnimationInstanceData& data = Context.GetInstanceData(*this);
	if (!data.Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTSAnimationTask: Actor is not set"));
		return EStateTreeRunStatus::Failed;
	}
	if (!data.Animation)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTSAnimationTask: Animation is not set"));
		return EStateTreeRunStatus::Failed;
	}
	ATSUnit* tsUnit = Cast<ATSUnit>(data.Actor);
	if(!tsUnit)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTSAnimationTask: Cannot cast Actor to TSUnit"));
		return EStateTreeRunStatus::Failed;
	}
	if(tsUnit->MeshID.IsMeshDataValid())
	{
		ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(tsUnit->MeshID, data.Animation, tsUnit->AnimPlaySettings);
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Failed;
}
