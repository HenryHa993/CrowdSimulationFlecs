// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "GameFramework/Actor.h"
#include "TSController.generated.h"

UCLASS()
class CROWDSIMULATION_API ATSController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATSController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category="TurboSequence")
	FTurboSequence_MeshSpawnData_Lf SpawnData;
	
	UPROPERTY(EditAnywhere, Category="TurboSequence")
	TObjectPtr<UAnimSequence> MeshAnimation;
	
	UPROPERTY(EditAnywhere, Category="TurboSequence")
	FTurboSequence_AnimPlaySettings_Lf MeshAnimationSettings = FTurboSequence_AnimPlaySettings_Lf();
};
