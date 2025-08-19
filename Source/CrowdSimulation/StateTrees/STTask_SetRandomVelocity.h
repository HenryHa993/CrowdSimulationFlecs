#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "STTask_SetRandomVelocity.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIMULATION_API FSetRandomVelocityTaskInstanceData
{
	GENERATED_BODY()

	// Pawn this task is associated with
	UPROPERTY(BlueprintReadWrite, Category=Context, meta=(Context))
	TObjectPtr<APawn> Actor;

	// Input speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0.f;

	// Output vector w/ random direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Task)
	FVector RandomVelocity = FVector::ZeroVector;
};

USTRUCT()
struct CROWDSIMULATION_API FSTTask_SetRandomVelocity : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	// For runtime
	using FInstanceDataType = FSetRandomVelocityTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override {return FSetRandomVelocityTaskInstanceData::StaticStruct();}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
