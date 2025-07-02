// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "ISMCrowdSimulationComponents.h"
#include "CrowdSimulation/Core/UnitConfigSet.h"
#include "ISMCrowdSimulationInitialisation.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UISMCrowdSimulationInitialisation : public UFlecsModuleBase, public IUnitConfigSet
{
	GENERATED_BODY()

public:
	virtual void Initialize(flecs::world& ECSWorld) override;
	
	uint32 CreateISMController(UWorld* World, UStaticMesh* StaticMesh, UMaterialInterface* Material, ISM_Map& Map);
};
