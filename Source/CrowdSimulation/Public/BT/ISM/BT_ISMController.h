// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BT_ISMController.generated.h"

UCLASS()
class CROWDSIMULATION_API ABT_ISMController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABT_ISMController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category=ISM)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
};
