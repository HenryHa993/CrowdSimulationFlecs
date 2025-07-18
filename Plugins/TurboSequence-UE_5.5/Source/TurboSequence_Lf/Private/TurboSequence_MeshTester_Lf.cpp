// Copyright Lukas Fratzl, 2022-2024. All Rights Reserved.


#include "TurboSequence_MeshTester_Lf.h"

#include "TurboSequence_Manager_Lf.h"


// Sets default values
ATurboSequence_MeshTester_Lf::ATurboSequence_MeshTester_Lf()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurboSequence_MeshTester_Lf::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurboSequence_MeshTester_Lf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TestMesh(DeltaTime);
}

void ATurboSequence_MeshTester_Lf::TestMesh(float DeltaTime)
{
	// Hash comparison to detect spawn changes
	if (SpawnData.GetHash() != LastSpawnData.GetHash())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn data not matched -- refresh mesh"));

		LastSpawnData = SpawnData;

		// Persistent mesh update context
		// CurrentMeshID maintained and monitored
		if (CurrentMeshID.IsMeshDataValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Mesh valid -- removing instance"));
			
			ATurboSequence_Manager_Lf::RemoveInstanceFromUpdateGroup_Concurrent(
				MeshUpdateContext.GroupIndex, CurrentMeshID);

			ATurboSequence_Manager_Lf::RemoveSkinnedMeshInstance_GameThread(CurrentMeshID, GetWorld());
		}

		// Check spawn data valid
		if (SpawnData.IsSpawnDataValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn data valid -- adding instance"));

			// Add new instance
			CurrentMeshID = ATurboSequence_Manager_Lf::AddSkinnedMeshInstance_GameThread(
				SpawnData, GetActorTransform(), GetWorld());
			// If instance valid, re-add to update group
			if (CurrentMeshID.IsMeshDataValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Instance added valid -- play animation"));

				// Should be done by the TS_AddInstance event structure
				ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(
					MeshUpdateContext.GroupIndex, CurrentMeshID);

				// Should be done via changes in the state machine
				CurrentAnimationID = ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(
					CurrentMeshID, MeshAnimation, MeshAnimationSettings);
			}
		}
	}

	if(TestFlag)
	{
		TestTimer -= DeltaTime;		
	}

	if(TestTimer < 0 && TestFlag)
	{
		CurrentAnimationID = ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(
					CurrentMeshID, SecondMeshAnimation, MeshAnimationSettings);

		TestFlag = false;
	}

	// Should be resolved by 
	ATurboSequence_Manager_Lf::SolveMeshes_GameThread(DeltaTime, GetWorld(), MeshUpdateContext);
}
