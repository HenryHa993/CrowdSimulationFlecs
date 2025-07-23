// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "ISM_Systems.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UISM_Systems : public UFlecsModuleBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(flecs::world& ECSWorld) override;
};
