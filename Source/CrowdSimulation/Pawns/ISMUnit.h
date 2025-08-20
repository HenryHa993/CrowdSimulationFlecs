// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "CrowdSimulation/ISM/ISMController.h"
#include "ISMUnit.generated.h"

UCLASS()
class CROWDSIMULATION_API AISMUnit : public AUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AISMUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	AISMController* ISMController = nullptr;

	UPROPERTY(EditAnywhere)
	int32 Index;
};
