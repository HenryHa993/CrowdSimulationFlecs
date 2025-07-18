// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "CrowdSimulation/Core/UnitConfigSet.h"
#include "TSCrowdSimulationInitialisation.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UTSCrowdSimulationInitialisation : public UFlecsModuleBase, public IUnitConfigSet
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
