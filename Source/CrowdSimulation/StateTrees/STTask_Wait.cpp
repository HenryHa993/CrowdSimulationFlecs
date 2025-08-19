#include "STTask_Wait.h"

EStateTreeRunStatus FSTTask_Wait::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FWaitInstanceData& data = Context.GetInstanceData(*this);
	data.ElapsedTime = 0.0f;
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Wait::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FWaitInstanceData& data = Context.GetInstanceData(*this);
	data.ElapsedTime += DeltaTime;
	if (data.ElapsedTime >= data.WaitTime)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}