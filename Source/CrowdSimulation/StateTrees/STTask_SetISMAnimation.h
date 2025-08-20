#pragma once
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "STTask_SetISMAnimation.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIMULATION_API FSetISMAnimationInstanceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category=Context, meta=(Context))
	TObjectPtr<APawn> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Task)
	float StartFrame = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Task)
	float EndFrame = 0.f;
};

USTRUCT()
struct CROWDSIMULATION_API FSTTask_SetISMAnimation : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSetISMAnimationInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FSetISMAnimationInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};