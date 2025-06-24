// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSimulationBootstrap.h"


// Sets default values
ACrowdSimulationBootstrap::ACrowdSimulationBootstrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACrowdSimulationBootstrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrowdSimulationBootstrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACrowdSimulationBootstrap::Bootstrap(flecs::world& ECSWorld)
{
	for(auto moduleType : FlecsModules)
	{
		auto module = NewObject<UFlecsModuleBase>(this, moduleType);
		module->Initialize(ECSWorld);
	}
}

