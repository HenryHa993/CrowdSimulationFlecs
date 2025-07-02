// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsBootstrap.h"
#include "FlecsModuleBase.h"
#include "UnitConfig.h"
#include "CrowdSimulationBootstrap.generated.h"

UCLASS()
class CROWDSIMULATION_API ACrowdSimulationBootstrap : public AFlecsBootstrap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACrowdSimulationBootstrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	UUnitConfig* UnitConfig;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UFlecsModuleBase>> FlecsModules;

protected:
	virtual void Bootstrap(flecs::world& ECSWorld) override;
};
