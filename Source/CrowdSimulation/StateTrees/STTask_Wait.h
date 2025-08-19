#pragma once
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "STTask_Wait.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIMULATION_API FWaitInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Task)
	float WaitTime = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category=Task)
	float ElapsedTime = 0.0f;
};

USTRUCT()
struct CROWDSIMULATION_API FSTTask_Wait : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FWaitInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FWaitInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};