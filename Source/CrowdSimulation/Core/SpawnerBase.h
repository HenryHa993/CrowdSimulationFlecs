// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerBase.generated.h"

UCLASS()
class CROWDSIMULATION_API ASpawnerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Spawn(int Columns, int Rows, float GridSize);

	UFUNCTION(BlueprintCallable)
	virtual void Despawn();

public:
	UPROPERTY(EditAnywhere)
	bool SpawnOnBeginPlay = false;

	UPROPERTY(EditAnywhere)
	int InitRows = 10;

	UPROPERTY(EditAnywhere)
	int InitColumns = 10;

	UPROPERTY(EditAnywhere)
	float InitGridSize;
};
