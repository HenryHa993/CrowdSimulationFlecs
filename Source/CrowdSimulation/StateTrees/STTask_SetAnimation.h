#pragma once
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "STTask_SetAnimation.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIMULATION_API FSetAnimationInstanceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category=Context, meta=(Context))
	TObjectPtr<APawn> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Task)
	TObjectPtr<UAnimSequence> Animation;
};

USTRUCT()
struct CROWDSIMULATION_API FSTTask_SetAnimation : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSetAnimationInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FSetAnimationInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};