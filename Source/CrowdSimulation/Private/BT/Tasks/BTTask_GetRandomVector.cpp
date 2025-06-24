// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Tasks/BTTask_GetRandomVector.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_GetRandomVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(BlackboardComponent)
	{
		// Generate random vector
		// Todo might want to normalise these speed values so animations don't seem jank
		FVector RandomVector;
		RandomVector.X = FMath::FRandRange(MinRange.X, MaxRange.X);
		RandomVector.Y = FMath::FRandRange(MinRange.Y, MaxRange.Y);
		RandomVector.Z = FMath::FRandRange(MinRange.Z, MaxRange.Z);

		// Set blackboard value
		BlackboardComponent->SetValueAsVector(OutputVectorKey.SelectedKeyName, RandomVector);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
