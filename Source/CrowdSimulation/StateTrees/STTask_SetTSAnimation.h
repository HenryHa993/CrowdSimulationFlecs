#pragma once
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "STTask_SetTSAnimation.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIMULATION_API FSetTSAnimationInstanceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category=Context, meta=(Context))
	TObjectPtr<APawn> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Task)
	TObjectPtr<UAnimSequence> Animation;
};

USTRUCT()
struct CROWDSIMULATION_API FSTTask_SetTSAnimation : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSetTSAnimationInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FSetTSAnimationInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};