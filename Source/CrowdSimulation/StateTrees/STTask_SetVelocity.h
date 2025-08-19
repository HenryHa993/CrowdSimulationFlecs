#pragma once
#include "StateTreeTaskBase.h"
#include "StateTreeExecutionContext.h"
#include "STTask_SetVelocity.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIMULATION_API FSetVelocityInstanceData
{
	GENERATED_BODY()

	// Pawn this task is associated with
	UPROPERTY(BlueprintReadWrite, Category=Context, meta=(Context))
	TObjectPtr<APawn> Actor;

	// Output vector w/ random direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity = FVector::ZeroVector;
};

USTRUCT()
struct CROWDSIMULATION_API FSTTask_SetVelocity : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSetVelocityInstanceData;

	virtual const UStruct* GetInstanceDataType() const override {return FSetVelocityInstanceData::StaticStruct();}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
