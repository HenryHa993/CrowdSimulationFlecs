// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "CrowdSimulation/Core/UnitConfig.h"
#include "Core_Components.generated.h"

// Basic Components
struct WorldRef
{
	UWorld* Value;
};

struct Transform
{
	FTransform Value;
};

struct Velocity
{
	FVector Value;
};

// AI State Machine
enum class FSM_State
{
	Wander,
	Wait
};

enum class FSM_Status
{
	Enter,
	Running,
	Exit
};

struct WanderStateData
{
	float CurrentWanderDuration;
};

struct WaitStateData
{
	float CurrentWaitDuration;
};

// 
struct UnitConfigRef
{
	UUnitConfig* Value;
};

// Phase tag
struct Despawn{};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UCore_Components : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
