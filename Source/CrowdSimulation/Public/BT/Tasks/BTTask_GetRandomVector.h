// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomVector.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UBTTask_GetRandomVector : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FVector MinRange;
	
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FVector MaxRange;
	
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector OutputVectorKey;
};
