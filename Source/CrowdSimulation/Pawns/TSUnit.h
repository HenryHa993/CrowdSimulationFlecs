// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "UnitBase.h"
#include "TSUnit.generated.h"

UCLASS()
class CROWDSIMULATION_API ATSUnit : public AUnitBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATSUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FTurboSequence_MinimalMeshData_Lf MeshID;
	FTurboSequence_AnimPlaySettings_Lf AnimPlaySettings;
};
