// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DevGuiSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UDevGuiSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	UDevGuiSubsystem();

public:
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetWorld(); }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always; }
	virtual bool IsTickableWhenPaused() const override { return true; }
	
	void MainMenu();
	void SpawnMenu(bool& bShow);
	void RenderMenu(bool& bShow);
	void AIMenu(bool& bShow);
	void Stats(bool& bShow);

	void SpawnUnits();
	void DespawnUnits();

	void UpdateStats(float deltaTime);

public:
	// Main menu parameters
	bool bShowSpawnMenu = false;
	bool bShowRenderMenu = false;
	bool bShowAIMenu = false;
	bool bShowStats = false;
	
	// Spawning parameters
	int NumUnits = 100;
	float DistanceBetweenUnits = 1000.0f;

	// Rendering parameters
	const char* RenderingItems[3];
	int SelectedRenderingItem;

	// Performance stats
	float FrameTime;
	float FramesPerSecond;
	float FrameTimes[120];
	int FrameTimesOffset = 0;
	float FPSs[120];
	int FPSsOffset = 0;
};
