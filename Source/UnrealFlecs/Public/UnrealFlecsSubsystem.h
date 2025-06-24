// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnrealFlecsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREALFLECS_API UUnrealFlecsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	flecs::world* GetECSWorld() const;

protected:
	flecs::world* ECSWorld = nullptr;

	FTickerDelegate OnTickDelegate;
	FTSTicker::FDelegateHandle OnTickHandle;

private:
	bool Tick(float DeltaTime);
};
