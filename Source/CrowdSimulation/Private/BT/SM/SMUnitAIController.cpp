// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/SM/SMUnitAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
ASMUnitAIController::ASMUnitAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
}

// Called when the game starts or when spawned
void ASMUnitAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void ASMUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(BlackboardComponent && BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*(BehaviorTree->GetBlackboardAsset()));
	}
}

