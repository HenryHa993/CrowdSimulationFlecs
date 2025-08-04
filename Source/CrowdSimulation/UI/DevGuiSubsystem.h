// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DevGuiSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UDevGuiSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetWorld(); }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always; }
	virtual bool IsTickableWhenPaused() const override { return true; }

	// UI Examples
	bool TickFinal();
	void ActorDebugger(bool& bActorDebuggerOpened);
	void HelloWorldTick();
	
	void MainMenu();
	void SpawnMenu(bool& bShow);
	void RenderMenu(bool& bShow);
	void AIMenu(bool& bShow);
	void Stats(bool& bShow);

	void SpawnUnits();
	void DespawnUnits();

public:
	// Main menu parameters
	bool bShowSpawnMenu = false;
	bool bShowRenderMenu = false;
	bool bShowAIMenu = false;
	bool bShowStats = false;
	
	// Spawning parameters
	int NumUnits = 100;
	float DistanceBetweenUnits = 1000.0f;
};
