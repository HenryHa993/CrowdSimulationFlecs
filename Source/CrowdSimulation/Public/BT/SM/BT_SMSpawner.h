// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SpawnerBase.h"
#include "BT_SMSpawner.generated.h"

UCLASS()
class CROWDSIMULATION_API ABT_SMSpawner : public ASpawnerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABT_SMSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Spawn(int Columns, int Rows, float GridSize) override;

	virtual void Despawn() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Unit;
};
