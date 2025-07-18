// Fill out your copyright notice in the Description page of Project Settings.


#include "TSController.h"

#include "TurboSequence_Manager_Lf.h"


// Sets default values
ATSController::ATSController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATSController::BeginPlay()
{
	Super::BeginPlay();

	// Create mesh instance
	const FTurboSequence_MinimalMeshData_Lf& instance = ATurboSequence_Manager_Lf::AddSkinnedMeshInstance_GameThread(SpawnData, GetActorTransform(), GetWorld());
	// Once mesh is valid, we can do stuff
	if(instance.IsMeshDataValid())
	{
		// Add instance to update group -- Can we add multiple to it?
		ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(0, instance);
		// Why are we saving the animation and not using it for anything?
		const FTurboSequence_AnimMinimalCollection_Lf& animation = ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(instance, MeshAnimation, MeshAnimationSettings);
	}
}

// Called every frame
void ATSController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Generate an update context which specifies the group we are solving on the game thread
	FTurboSequence_UpdateContext_Lf updateContext = FTurboSequence_UpdateContext_Lf();
	updateContext.GroupIndex = 0;

	ATurboSequence_Manager_Lf::SolveMeshes_GameThread(DeltaTime, GetWorld(), updateContext);
}

