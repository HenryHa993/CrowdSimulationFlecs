// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealFlecsSubsystem.h"

void UUnrealFlecsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	OnTickDelegate = FTickerDelegate::CreateUObject(this, &UUnrealFlecsSubsystem::Tick);
	OnTickHandle = FTSTicker::GetCoreTicker().AddTicker(OnTickDelegate);

	ECSWorld = new flecs::world;

	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has started."));
	
	Super::Initialize(Collection);
}

void UUnrealFlecsSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(OnTickHandle);

	if(ECSWorld)
	{
		delete ECSWorld;
		ECSWorld = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has shut down."));

	Super::Deinitialize();
}

flecs::world* UUnrealFlecsSubsystem::GetECSWorld() const
{
	return ECSWorld;
}

bool UUnrealFlecsSubsystem::Tick(float DeltaTime)
{
	if(ECSWorld) ECSWorld->progress(DeltaTime);
	return true;
}
