// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "flecs.h"
#include "GameFramework/Actor.h"
#include "FlecsBootstrap.generated.h"

UCLASS()
class UNREALFLECS_API AFlecsBootstrap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlecsBootstrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void Bootstrap(flecs::world& ECSWorld);
};
