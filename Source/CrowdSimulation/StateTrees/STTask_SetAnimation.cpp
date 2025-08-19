#include "STTask_SetAnimation.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"

EStateTreeRunStatus FSTTask_SetAnimation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FSetAnimationInstanceData& data = Context.GetInstanceData(*this);
	if (!data.Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimationTask: Actor is not set"));
		return EStateTreeRunStatus::Failed;
	}
	if (!data.Animation)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimationTask: Animation is not set"));
		return EStateTreeRunStatus::Failed;
	}
	USkeletalMeshComponent* mesh = data.Actor->FindComponentByClass<USkeletalMeshComponent>();
	if (!mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetAnimationTask: SkeletalMeshComponent not found"));
		return EStateTreeRunStatus::Failed;
	}
	mesh->PlayAnimation(data.Animation, false);
	return EStateTreeRunStatus::Succeeded;
}