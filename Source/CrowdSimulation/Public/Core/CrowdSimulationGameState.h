// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerBase.h"
#include "GameFramework/GameStateBase.h"
#include "CrowdSimulationGameState.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API ACrowdSimulationGameState : public AGameStateBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ASpawnerBase>> SpawnerArray;
};
