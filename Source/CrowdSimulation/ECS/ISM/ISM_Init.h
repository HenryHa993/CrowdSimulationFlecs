// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "ISM_Components.h"
#include "CrowdSimulation/Core/UnitConfigSet.h"
#include "ISM_Init.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UISM_Init : public UFlecsModuleBase, public IUnitConfigSet
{
	GENERATED_BODY()

public:
	virtual void Initialize(flecs::world& ECSWorld) override;
	
	uint32 CreateISMController(UWorld* World, UStaticMesh* StaticMesh, UMaterialInterface* Material, ISM_Map& Map);
};
