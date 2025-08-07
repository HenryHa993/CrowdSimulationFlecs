// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "CrowdSimulation/Core/UnitConfigSet.h"
#include "TS_Init.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UTS_Init : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
