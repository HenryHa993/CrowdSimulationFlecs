// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "RenderCore.h"
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

	void UpdateStats(float DeltaTime);

public:
	// Main menu parameters
	bool bShowSpawnMenu = false;
	bool bShowRenderMenu = false;
	bool bShowAIMenu = false;
	bool bShowStats = false;

	int EntityCount = 0;
	
	float SpawnX = 0.f;
	float SpawnY = 0.f;
	float SpawnZ = 0.f;
	
	// Spawning parameters
	int NumUnitsX = 16;
	int NumUnitsY = 1;
	float DistanceBetweenUnitsX = 0.f;
	float DistanceBetweenUnitsY = 0.f;

	// Framework selection
	const char* FrameworkItems[2];
	int SelectedFrameworkItem;

	// Rendering parameters
	const char* RenderingItems[3];
	int SelectedRenderingItem;

	// Performance stats
	float FrameTime;
	float FrameTimes[120];
	int FrameTimesOffset = 0;

	float FramesPerSecond;
	float FPSs[120];
	int FPSsOffset = 0;

	float GameThreadTime;
	float GameThreadTimes[120];
	int GameThreadTimesOffset = 0;

	float RenderThreadTime;
	float RenderThreadTimes[120];
	int RenderThreadTimesOffset = 0;

	// Auto FPS Grapher
	float AutoFPSTimer = 60.f;
	float ElapsedAutoFPSTime = 0.f;
	bool bAutoFPSStarted = false;

	// Auto Game Thread Profiler
	float AutoStatCaptureTimer = 60.f;
	float ElapsedAutoStatCaptureTime = 0.f;
	bool bAutoStatCaptureStarted = false;

	// Auto Bookmarker
	float AutoBookmarkTimer = 60.f;
	float ElapsedAutoBookmarkTime = 0.f;
	bool bAutoBookmarkStarted = false;
};
