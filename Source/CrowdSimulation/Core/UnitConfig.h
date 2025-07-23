// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "Engine/DataAsset.h"
#include "UnitConfig.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UUnitConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="ISM Config")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category="ISM Config")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category="TS Config")
	FTurboSequence_MeshSpawnData_Lf SpawnData;

	UPROPERTY(EditAnywhere, Category="TS Config")
	UAnimSequence* IdleAnim;

	UPROPERTY(EditAnywhere, Category="TS Config")
	UAnimSequence* WalkingAnim;
};
