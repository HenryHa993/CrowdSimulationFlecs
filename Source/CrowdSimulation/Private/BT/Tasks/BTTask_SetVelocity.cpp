// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Tasks/BTTask_SetVelocity.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PawnMovementComponent.h"

EBTNodeResult::Type UBTTask_SetVelocity::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	
	AAIController* controller = OwnerComp.GetAIOwner();
	APawn* pawn = controller->GetPawn();
	UPawnMovementComponent* movementComponent = pawn->GetMovementComponent();

	if(blackboardComponent && movementComponent)
	{
		movementComponent->Velocity = blackboardComponent->GetValueAsVector(InputVectorKey.SelectedKeyName);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
