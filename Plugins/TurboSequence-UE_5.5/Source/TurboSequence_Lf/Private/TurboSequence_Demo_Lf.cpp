// Copyright Lukas Fratzl, 2022-2024. All Rights Reserved.


#include "TurboSequence_Demo_Lf.h"

#include "TurboSequence_Manager_Lf.h"


// Sets default values
ATurboSequence_Demo_Lf::ATurboSequence_Demo_Lf()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.TickGroup = TG_StartPhysics;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATurboSequence_Demo_Lf::BeginPlay()
{
	Super::BeginPlay();

	// FDemoAssetData_Lf defines a bunch of animation properties which build up an archetype
	// So as long as some archetypes populate the array, it will run
	if (AssetData.Num())
	{
		UpdateGroupIndex = QualityGroupIndex + 1;
		DemoComponentHeight = GetActorTransform().GetLocation().Z;

		AssetDataRuntime.Empty();

		// For each archetype -- Which is a collection of animation spawn data, and animation data
		for (FDemoAssetData_Lf& Asset : AssetData)
		{
			// CategorizedRootData contains an array of TS Mesh - material combinations
			Asset.CategorizedRootData.CategorizedData.Empty();
			// Iterate through collection of TS Meshes and stuff for spawning -- Spawn data
			for (const FDemoMeshSpawnData_Lf& RootAsset : Asset.RootAssets)
			{
				if (IsValid(RootAsset.Asset))
				{
					// So I guess some of the archetypes can be excluded?
					if (!Asset.bExcludeFromSystem && !RootAsset.bExclude)
					{
						// Add it to that categorised spawn data array
						Asset.CategorizedRootData.CategorizedData.Add(RootAsset);
					}
				}
			}
			// Empty customisable data array
			Asset.CategorizeCustomizableData.Empty();
			// Lol this also contains spawn TS Mesh stuff
			for (const FDemoMeshSpawnData_Lf& CustomizableAsset : Asset.CustomizableAssets)
			{
				// Check if TS Mesh is fine
				if (IsValid(CustomizableAsset.Asset))
				{
					// And then whether or not to exclude asset
					if (!Asset.bExcludeFromSystem && !CustomizableAsset.bExclude)
					{
						// If there is NOT a category for the archetype
						if (!Asset.CategorizeCustomizableData.Contains(CustomizableAsset.CategoryName))
						{
							// Create the category
							FDemoCustomizationContainer_Lf Data;
							Data.CategorizedData.Add(CustomizableAsset);
							Asset.CategorizeCustomizableData.Add(CustomizableAsset.CategoryName, Data);
						}
						else
						{
							// Otherwise add it to the map of categorised data
							Asset.CategorizeCustomizableData[CustomizableAsset.CategoryName].CategorizedData.Add(
								CustomizableAsset);
						}
					}
				}
			}

			// Add asset to runtime array
			if (!Asset.bExcludeFromSystem)
			{
				AssetDataRuntime.Add(&Asset);
			}
		}

		// All assets are excluded, do not continue
		if (!AssetDataRuntime.Num())
		{
			return;
		}

		// Round meshes to spawn (I think they are spawning in batches)
		int32 AmountMeshSqrt = FMath::RoundToInt(FMath::Sqrt(static_cast<float>(AmountOfMeshesToSpawn)));
		if (AmountOfMeshesToSpawn == 1)
		{
			AmountMeshSqrt = 1;
		}
		CurrentMeshCount_Internal = AmountMeshSqrt * AmountMeshSqrt;
		const float Offset = AmountMeshSqrt * DistanceBetweenMeshes * 0.5f;
		LastSpawnLocation_Internal = FVector(Offset, Offset, 0);

		if (CurrentMeshCount_Internal)
		{
			UE_LOG(LogTurboSequence_Lf, Warning, TEXT("Spawning -> %d Characters, in %d Frames"),
			       CurrentMeshCount_Internal, SpawnFrameDelay + AmountMeshSqrt);

			SpawnFrameDelay_Internal = SpawnFrameDelay;

			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green,
			                                 FString::Printf(
				                                 TEXT("Spawning -> %d Characters, in %d Frames"),
				                                 CurrentMeshCount_Internal, SpawnFrameDelay + AmountMeshSqrt));

			// Set timer to spawn a bunch of meshes
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATurboSequence_Demo_Lf::SpawnCharactersDelayed);
		}
	}
}

// Called every frame
void ATurboSequence_Demo_Lf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AssetDataRuntime.Num())
	{
		if (!ATurboSequence_Manager_Lf::GlobalLibrary.UpdateGroups.IsValidIndex(UpdateGroupIndex))
		{
			return;
		}

		// Resize array to match number of update groups
		UpdateGroupDeltaTimes.SetNum(ATurboSequence_Manager_Lf::GlobalLibrary.UpdateGroups.Num());

		// todo Why does each update group maintain their own delta time?
		for (float& Delta : UpdateGroupDeltaTimes)
		{
			Delta += DeltaTime;
		}

		// Get camera location and rotation
		FVector CameraLocation = FVector::ZeroVector;
		FRotator CameraRotation = FRotator::ZeroRotator;
		if (IsValid(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)))
		{
			CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
			CameraRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation();
		}

		// Determines which mesh instances are within the quality update group
		TMap<FTurboSequence_MinimalMeshData_Lf, bool> SwitchingGroups;

		// Generate a new update context
		FTurboSequence_UpdateContext_Lf UpdateContext = FTurboSequence_UpdateContext_Lf();
		UpdateContext.GroupIndex = UpdateGroupIndex;
		//UpdateContext.bCollectGarbageThisFrame = bCollectGarbageThisFrame;

		// Determines how the instances of an update group move, rotate and are animated
		SolveGroup(UpdateGroupIndex, UpdateGroupDeltaTimes[UpdateGroupIndex], CameraRotation,
		           CameraLocation, SwitchingGroups);

		if (bUpdateManager)
		{
			// Solve Background Group
			ATurboSequence_Manager_Lf::SolveMeshes_GameThread(UpdateGroupDeltaTimes[UpdateGroupIndex], GetWorld(),
			                                                  UpdateContext);

			// Foreground Group
			SolveGroup(QualityGroupIndex, DeltaTime, CameraRotation, CameraLocation, SwitchingGroups);

			// With the switching groups configured
			// Adjust which meshes you want to move and remove from the quality update group
			for (const TTuple<FTurboSequence_MinimalMeshData_Lf, bool>& Group : SwitchingGroups)
			{
				FDemoMeshWrapper_Lf& Mesh = Meshes[Group.Key];
				// If true
				if (Group.Value)
				{
					// Remove instance from update group and put it into quality update group
					// So switch group represents that meshes instances you want to move into the quality update group
					ATurboSequence_Manager_Lf::RemoveInstanceFromUpdateGroup_Concurrent(
						Mesh.CurrentUpdateGroupIndex, Mesh.MeshData);
					Mesh.CurrentUpdateGroupIndex = QualityGroupIndex;
					ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(
						Mesh.CurrentUpdateGroupIndex, Mesh.MeshData);
				}
				// Otherwise put it into a default update group
				else
				{
					ATurboSequence_Manager_Lf::RemoveInstanceFromUpdateGroup_Concurrent(
						QualityGroupIndex, Mesh.MeshData);
					Mesh.CurrentUpdateGroupIndex = Mesh.DefaultUpdateGroupIndex;
					ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(
						Mesh.CurrentUpdateGroupIndex, Mesh.MeshData);
				}
			}

			// High Quality Solving
			// If any are in the high quality solving group
			if (ATurboSequence_Manager_Lf::GlobalLibrary.UpdateGroups[QualityGroupIndex].RawIDs.Num())
			{
				// Solve them on the gamethread
				UpdateContext.GroupIndex = QualityGroupIndex;

				ATurboSequence_Manager_Lf::SolveMeshes_GameThread(DeltaTime, GetWorld(), UpdateContext);
			}
		}

		UpdateGroupDeltaTimes[UpdateGroupIndex] = 0;

		UpdateGroupIndex++;
		UpdateGroupIndex = UpdateGroupIndex % ATurboSequence_Manager_Lf::GlobalLibrary.UpdateGroups.Num();
		UpdateGroupIndex = FMath::Clamp(UpdateGroupIndex, QualityGroupIndex + 1,
		                                ATurboSequence_Manager_Lf::GlobalLibrary.UpdateGroups.Num());
	}
}

void ATurboSequence_Demo_Lf::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATurboSequence_Demo_Lf::SpawnCharactersDelayed()
{
	// If remaining frames to spawn, setup timer to spawn next tick
	if (SpawnFrameDelay_Internal)
	{
		SpawnFrameDelay_Internal--;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATurboSequence_Demo_Lf::SpawnCharactersDelayed);
		return;
	}

	
	const FVector& SpawnerLocation = GetActorLocation();
	//FCriticalSection CriticalSection;

	// If current mesh count larger than 0
	if (CurrentMeshCount_Internal > 0)
	{
		// Offset last spawn location
		LastSpawnLocation_Internal += FVector(-DistanceBetweenMeshes, 0, 0);

		// Square root the amount of meshes to spawn
		int32 AmountCharactersSqrt = FMath::RoundToInt(FMath::Sqrt(static_cast<float>(AmountOfMeshesToSpawn)));
		if (AmountOfMeshesToSpawn == 1)
		{
			AmountCharactersSqrt = 1;
		}
		//const int32 MaxNumMeshes = AmountCharactersSqrt * AmountCharactersSqrt;

		// Add a unique mesh-material instance into the world
		for (int32 Y = 0; Y < AmountCharactersSqrt; ++Y)
		{
			// Generate an offset
			FVector Location = FVector(LastSpawnLocation_Internal.X,
			                           Y * -DistanceBetweenMeshes + LastSpawnLocation_Internal.Y, 0) + SpawnerLocation;
			// Reduce mesh count (I assume this represents the number of different meshes being spawned?)
			CurrentMeshCount_Internal--;

			FTransform Transform = FTransform(Location);

			FTurboSequence_MeshSpawnData_Lf SpawnData;

			// Get a random asset (IK details, settings and collection of TS Meshes)
			int32 AssetIndex = FMath::RandRange(0, AssetDataRuntime.Num() - 1);

			FDemoAssetData_Lf* Asset = AssetDataRuntime[AssetIndex];

			// Populate SpawnData with a random mesh-material combination
			GetRandomMeshSpawnData(SpawnData, Asset->CategorizedRootData, Asset->CategorizeCustomizableData,
			                       Asset->FootprintAsset);

			// Add instance of SpawnData and check if valid
			if (const FTurboSequence_MinimalMeshData_Lf& MeshData =
					ATurboSequence_Manager_Lf::AddSkinnedMeshInstance_GameThread(SpawnData, Transform, GetWorld());
				MeshData.IsMeshDataValid()) // Very Important to check here if it's valid
			{
				// Populate Mesh, which is a wrapper around spawning data
				FDemoMeshWrapper_Lf Mesh = FDemoMeshWrapper_Lf();
				Mesh.MeshData = MeshData;
				Mesh.RandomRotationTimer = GET1_NUMBER;
				Mesh.AssetDataIndex = AssetIndex;

				// Get group index
				const int32 GroupIndex = ATurboSequence_Manager_Lf::GetNumOfAllMeshes_Concurrent() /
					NumInstancesPerUpdateGroup + QualityGroupIndex + 1;

				Mesh.DefaultUpdateGroupIndex = GroupIndex;
				Mesh.CurrentUpdateGroupIndex = GroupIndex;

				// Add instance to a specific update group
				ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(GroupIndex, MeshData);

				// Add to map the mesh instance and the demo mesh wrapper
				Meshes.Add(MeshData, Mesh);

				// Calculate a world transform offset
				// todo: This is pretty useful for getting transform
				const FTransform& WorldTransformIncludingOffset = Asset->SpawnOffsetTransform *
					ATurboSequence_Manager_Lf::GetMeshWorldSpaceTransform_Concurrent(Mesh.MeshData); // Use mesh instance to access it's transform
				ATurboSequence_Manager_Lf::SetMeshWorldSpaceTransform_Concurrent(
					Mesh.MeshData, WorldTransformIncludingOffset); // Then set it's transform to the calculated offset
			}
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATurboSequence_Demo_Lf::SpawnCharactersDelayed);
	}
	// If internal mesh count is 0, print debug message
	else
	{
		uint32 NumMeshes = ATurboSequence_Manager_Lf::GetNumOfAllMeshes_Concurrent();

		UE_LOG(LogTurboSequence_Lf, Warning, TEXT("Spawning -> %d Meshes"), NumMeshes);

		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Green,
		                                 FString::Printf(TEXT("Spawning -> %d Meshes"), NumMeshes));
	}
}


// void ATurboSequence_Demo_Lf::BindToInput()
// {
// 	// Initialize our component
// 	InputComponent = NewObject<UInputComponent>(this);
// 	InputComponent->RegisterComponent();
// 	if (InputComponent)
// 	{
// 		// Bind inputs here
// 		// InputComponent->BindAction("Jump", IE_Pressed, this, &AUnrealisticPawn::Jump);
// 		// etc...
//
// 		// Now hook up our InputComponent to one in a Player
// 		// Controller, so that input flows down to us
// 		EnableInput(GetWorld()->GetFirstPlayerController());
//
// 		// Bind an action to it
// 		InputComponent->BindAction
// 		(
// 			"Test_LeftMouseButton", // The input identifier (specified in DefaultInput.ini)
// 			IE_Pressed, // React when button pressed (or on release, etc., if desired)
// 			this, // The object instance that is going to react to the input
// 			&ATurboSequence_Demo_Lf::LeftMouseButtonPressed // The function that will fire when input is received
// 		);
// 	}
// }
//
// void ATurboSequence_Demo_Lf::LeftMouseButtonPressed()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Pressed"));
// }

/*
 * Appears to be where multithreading occurs in order to
 */
void ATurboSequence_Demo_Lf::SolveGroup(int32 GroupIndex,
                                        float DeltaTime, const FRotator& CameraRotation,
                                        const FVector& CameraLocation,
                                        TMap<FTurboSequence_MinimalMeshData_Lf, bool>& SwitchingGroups)
{
	// Retrieve number of mesh data from given group index
	const int32 NumMeshes = ATurboSequence_Manager_Lf::GetNumMeshDataInUpdateGroup_Concurrent(GroupIndex);
	if (!NumMeshes)
	{
		return;
	}

	// For multithreading
	FCriticalSection CriticalSection;

	// Get number of CPU threads - 1 (I assume this will be later used for iteration)
	int16 NumThreads = FTurboSequence_Helper_Lf::NumCPUThreads() - GET1_NUMBER;

	// Calculate the number of meshes per thread to be updated(?)
	int32 NumMeshesPerThread = FMath::CeilToInt(
		static_cast<float>(NumMeshes) / static_cast<float>(NumThreads));

	// The work each thread is doing
	// todo what work is it doing?
	// THIS SEEMS TO BE A MASSIVE EXAMPLE OF MULTITHREADING -- SHOULD REFERENCE THIS WHEN TRYING TO IMPLEMENT IT FOR ECS
	// This appears to be capturing everything within the current scope
	ParallelFor(NumThreads, [&](int32 ThreadsIndex)
	{
		// Finding the ranges of which to operate on the meshes
		const int32 MeshBaseIndex = ThreadsIndex * NumMeshesPerThread;
		const int32 MeshBaseNum = MeshBaseIndex + NumMeshesPerThread;

		// Iterate through the mesh ranges
		for (int32 Index = MeshBaseIndex; Index < MeshBaseNum; ++Index)
		{
			// If meshes are larger than the total number of meshes, exit
			if (Index >= NumMeshes)
			{
				break;
			}

			// Index represents the numbered mesh instance. Using the update group index, and the mesh instance index we
			// can get the mesh instance itself
			const FTurboSequence_MinimalMeshData_Lf& MeshData =
				ATurboSequence_Manager_Lf::GetMeshDataInUpdateGroupFromIndex_Concurrent(GroupIndex, Index);

			// Validation checks before operating on the mesh data
			if (!MeshData.IsMeshDataValid() || (MeshData.IsMeshDataValid() && !Meshes.Contains(MeshData)))
			{
				continue;
			}

			// Get the wrapper from the map
			FDemoMeshWrapper_Lf& Mesh = Meshes[MeshData];
			//
			// SolveMesh(Mesh, CriticalSection, CameraRotation, CameraLocation, SwitchingGroups, DeltaTime);

			// Get demo asset data
			const FDemoAssetData_Lf* AssetCustomData = AssetDataRuntime[Mesh.AssetDataIndex];
			// Get ts mesh from raw ID
			const TObjectPtr<UTurboSequence_MeshAsset_Lf> MeshAsset =
				ATurboSequence_Manager_Lf::GetMeshAsset_RawID_Concurrent(
					Mesh.MeshData.RootMotionMeshID);

			// Here we make a simple Distance Updating in seconds, if DeltaTimeAccumulator is accumulated
			// higher than DistanceRatioSeconds we update the Mesh
			// this is truly optional and only here for optimization
			float CameraDistance = ATurboSequence_Manager_Lf::GetMeshClosestCameraDistance_Concurrent(
				Mesh.MeshData);
			float DistanceRatioSeconds = MeshAsset->bUseDistanceUpdating
				                             ? CameraDistance / 250000 * MeshAsset->DistanceUpdatingRatio
				                             : 0;

			// If we get over distance ration, we update the mesh
			// todo: this whole thing culls mesh updates depending on some sort of camera distance-time calculation
			Mesh.DeltaTimeAccumulator += DeltaTime;
			if (Mesh.DeltaTimeAccumulator > DistanceRatioSeconds)
			{
				// Moving mesh with root motion
				if (AssetCustomData->bUseRootMotion)
				{
					ATurboSequence_Manager_Lf::MoveMeshWithRootMotion_Concurrent(
						Mesh.MeshData, Mesh.DeltaTimeAccumulator, true, false);
				}
				// Using programmable speed meaning that animations have a certain associated speed
				else if (AssetCustomData->bUseProgrammableSpeed)
				{
					// Get animation sequence
					const TObjectPtr<UAnimSequence> AnimSequence =
						ATurboSequence_Manager_Lf::GetHighestPriorityPlayingAnimation_Concurrent(Mesh.MeshData);

					// If animation library is valid
					if (IsValid(MeshAsset->AnimationLibrary))
					{
						// Searches the animation library for the current animation speed
						FVector Speed = FVector::ZeroVector;
						for (FAnimationLibraryItem_Lf& AnimationLibraryItem : MeshAsset->AnimationLibrary->Animations)
						{
							// if the animation matches the current animation sequence, 
							if (AnimationLibraryItem.Animation == AnimSequence)
							{
								Speed = AnimationLibraryItem.AnimationSeed_CM_Per_Second; // Apparently how fast the animation represents
								break;
							}
						}

						// Get mesh transform from the instance
						FTransform MeshTransform = ATurboSequence_Manager_Lf::GetMeshWorldSpaceTransform_Concurrent(
							Mesh.MeshData);

						// Set location of transform
						MeshTransform.SetLocation(
							MeshTransform.GetLocation() + MeshTransform.GetRotation().RotateVector(Speed * DeltaTime));
						
						ATurboSequence_Manager_Lf::SetMeshWorldSpaceTransform_Concurrent(Mesh.MeshData, MeshTransform);
					}
				}
				// Fix transform to a certain height
				if (bKeepHeightOnSpawnLevel)
				{
					FTransform MeshTransform = ATurboSequence_Manager_Lf::GetMeshWorldSpaceTransform_Concurrent(
						Mesh.MeshData);

					FVector Location = MeshTransform.GetLocation();
					Location.Z = DemoComponentHeight;
					MeshTransform.SetLocation(Location);

					ATurboSequence_Manager_Lf::SetMeshWorldSpaceTransform_Concurrent(Mesh.MeshData, MeshTransform);
				}
				// Randomise rotation of mesh
				if (AssetCustomData->bUseRandomRotation)
				{
					// Reduce timer
					Mesh.RandomRotationTimer -= Mesh.DeltaTimeAccumulator;

					// If timer is up, rotate mesh yaw
					if (Mesh.RandomRotationTimer < 0)
					{
						Mesh.RandomRotationTimer = FMath::RandRange(2.0f, 7.0f);
						const float RandomYaw = FMath::RandRange(-180.0f, 180.0f);

						Mesh.RandomRotationYaw = RandomYaw;
					}

					FTransform MeshTransform = ATurboSequence_Manager_Lf::GetMeshWorldSpaceTransform_Concurrent(
						Mesh.MeshData);
					FRotator Rotation = MeshTransform.Rotator();

					// Lerp yaw
					Rotation.Yaw = FRotator::NormalizeAxis(
						FMath::Lerp(Rotation.Yaw, static_cast<double>(Mesh.RandomRotationYaw),
						            static_cast<double>(Mesh.DeltaTimeAccumulator)));

					// Setting rotation
					//MeshTransform.SetRotation(Rotation.Quaternion());
					ATurboSequence_Manager_Lf::SetMeshWorldSpaceLocationRotationScale_Concurrent(
						Mesh.MeshData, MeshTransform.GetLocation(), Rotation.Quaternion(), FVector::OneVector);
				}
				// 
				if (AssetCustomData->bUseRandomAnimation)
				{
					// A lot of things have timers on them lol
					Mesh.RandomAnimationTimer -= Mesh.DeltaTimeAccumulator;
					
					// Check timer, check validation
					if (Mesh.RandomAnimationTimer < 0)
					{
						if (IsValid(MeshAsset->AnimationLibrary)) // TODO: Remove
						{
							// If mesh is using blendspaces and is within camera view distance
							if (AssetCustomData->bUseBlendSpaces && CameraDistance < 15000)
							{
								// Check if there are blendspaces
								if (int32 NumBlendSpaces = MeshAsset->AnimationLibrary->BlendSpaces.Num())
								{
									int32 RandomBlendSpace = FMath::RandRange(0, NumBlendSpaces - 1);
									FTurboSequence_AnimPlaySettings_Lf PlaySettings =
										FTurboSequence_AnimPlaySettings_Lf();

									// If current blendspace is not valid, or current blendspace does not match random
									if (!Mesh.CurrentBlendSpace.IsAnimCollectionValid() || Mesh.CurrentBlendSpace.
										RootMotionMesh.BlendSpace != MeshAsset->AnimationLibrary->BlendSpaces[
											RandomBlendSpace])
									{
										// Play random blenspace 
										Mesh.CurrentBlendSpace = ATurboSequence_Manager_Lf::PlayBlendSpace_Concurrent(
											Mesh.MeshData, MeshAsset->AnimationLibrary->BlendSpaces[RandomBlendSpace],
											PlaySettings);
									}

									// Calculate and set blendspace position
									// todo not sure how randomising the blendspace position affects the animation
									Mesh.RandomBlendSpacePosition = FVector3f(
										FMath::RandRange(-200.0, 200.0), FMath::RandRange(-200.0, 200.0), 0);
									if (FVector3f::Distance(Mesh.RandomBlendSpacePosition, FVector3f::ZeroVector) < 100)
									{
										Mesh.RandomBlendSpacePosition = FVector3f::ZeroVector;
									}
									else
									{
										Mesh.RandomBlendSpacePosition = Mesh.RandomBlendSpacePosition.GetUnsafeNormal() * 200;
									}

									// todo: THIS TWEAK BLENDSPACE FUNCTION LOOKS USEFUL
									ATurboSequence_Manager_Lf::TweakBlendSpace_Concurrent(
										Mesh.CurrentBlendSpace, Mesh.RandomBlendSpacePosition);
								}
							}
							// If not using blendspaces OR not within camera distance
							else
							{
								// If there are animations
								if (int32 NumAnimations = MeshAsset->AnimationLibrary->Animations.Num())
								{
									// Randomly pick one
									int32 RandomAnimation = FMath::RandRange(0, NumAnimations - 1);
									if (NumAnimations > 1)
									{
										while (ATurboSequence_Manager_Lf::GetHighestPriorityPlayingAnimation_Concurrent(
												Mesh.MeshData) == MeshAsset->AnimationLibrary->Animations[
												RandomAnimation].
											Animation)
										{
											RandomAnimation = FMath::RandRange(0, NumAnimations - 1);
										}
									}
									// Then play the next animation
									FTurboSequence_AnimPlaySettings_Lf PlaySettings =
										FTurboSequence_AnimPlaySettings_Lf();
									Mesh.CurrentAnimation_0 = ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(
										Mesh.MeshData,
										MeshAsset->AnimationLibrary->Animations[RandomAnimation].Animation,
										PlaySettings);

									// If using layered animations
									if (AssetCustomData->bUseLayer)
									{
										// Get random animation
										int32 RandomAnimation2 = FMath::RandRange(0, NumAnimations - 1);
										if (NumAnimations > 1)
										{
											while (
												ATurboSequence_Manager_Lf::GetHighestPriorityPlayingAnimation_Concurrent(
													Mesh.MeshData) == MeshAsset->AnimationLibrary->Animations[
													RandomAnimation2].Animation)
											{
												RandomAnimation2 = FMath::RandRange(0, NumAnimations - 1);
											}
										}
										// Customise play settings to do layered animations
										// todo: SEE THIS FOR ANIMATION BLENDING
										// THen play
										PlaySettings.AnimationPlayTimeInSeconds = FMath::RandRange(0.0f, 0.5f);
										PlaySettings.BoneLayerMasks = AssetCustomData->BoneLayers;
										//PlaySettings.Animation = MeshAsset->AnimationLibrary->Animations[RandomAnimation2];
										Mesh.CurrentAnimation_1 = ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(
											Mesh.MeshData,
											MeshAsset->AnimationLibrary->Animations[RandomAnimation2].Animation,
											PlaySettings);
									}
								}
							}
						}

						// Randomise a bunch of timers
						Mesh.RandomAnimationTimer = FMath::RandRange(2.0f, 7.0f);
						Mesh.RandomAnimationData_0 = FMath::RandRange(0.9f, 1.75f);
						Mesh.RandomAnimationData_1 = FMath::RandRange(0.9f, 1.75f);
					}

					// No clue what is happening here
					if (AssetCustomData->bUseCustomData)
					{
						Mesh.CustomDataTimer -= Mesh.DeltaTimeAccumulator;
						if (Mesh.CustomDataTimer < 0)
						{
							Mesh.CustomDataTimer = FMath::RandRange(2.0f, 5.0f);

							const FLinearColor& RandomColor = FLinearColor::MakeRandomColor();

							ATurboSequence_Manager_Lf::SetCustomDataToInstance_Concurrent(
								Mesh.MeshData, 4, RandomColor.R);
							ATurboSequence_Manager_Lf::SetCustomDataToInstance_Concurrent(
								Mesh.MeshData, 5, RandomColor.G);
							ATurboSequence_Manager_Lf::SetCustomDataToInstance_Concurrent(
								Mesh.MeshData, 6, RandomColor.B);
						}
					}
				}


				// IK is expensive on the CPU, we only do IK for 100 Meters Radius around the camera
				// and only if the mesh is visible by the Camera Frustum
				// todo: CAMERA FRUSTRUM FUNCTION CAN HELP WITH ANIMATING
				if (AssetCustomData->bUseIK && CameraDistance < 10000 &&
					ATurboSequence_Manager_Lf::GetIsMeshVisibleInCameraFrustum_Concurrent(Mesh.MeshData, false))
				{
					const FTransform& OffsetTransform = AssetCustomData->SpawnOffsetTransform;

					const FTransform& MeshTransform =
						ATurboSequence_Manager_Lf::GetMeshWorldSpaceTransform_Concurrent(Mesh.MeshData) *
						OffsetTransform.Inverse();

					float Dot = FVector::DotProduct(CameraRotation.Vector(),
					                                MeshTransform.GetRotation().Vector());
					bool bIsInView = Dot < 0;

					Mesh.IKWeight = FTurboSequence_Helper_Lf::Clamp01(FMath::Lerp(
						Mesh.IKWeight, static_cast<float>(bIsInView),
						bIsInView ? 2.0f * Mesh.DeltaTimeAccumulator : 5.0f * Mesh.DeltaTimeAccumulator));


					// This Look At IK need to be done backwards like so many IKs because,
					// If I move first the head and add the movement from spine I end up with
					// a location and rotation offset on head
					// Another thing is we need to add DeltaTime into the IK Because GetIKTransform
					// is sometimes updating the Animation to stay up to date
					SolveLookAtIKBone(Mesh.MeshData, AssetCustomData->Spine1Bone, CameraLocation, Mesh.IKWeight,
					                  AssetCustomData->Spine1IKStiffness, DeltaTime,
					                  OffsetTransform);
					SolveLookAtIKBone(Mesh.MeshData, AssetCustomData->Spine2Bone, CameraLocation, Mesh.IKWeight,
					                  AssetCustomData->Spine2IKStiffness, DeltaTime,
					                  OffsetTransform);
					SolveLookAtIKBone(Mesh.MeshData, AssetCustomData->NeckBone, CameraLocation, Mesh.IKWeight,
					                  AssetCustomData->NeckIKStiffness, DeltaTime,
					                  OffsetTransform);
					SolveLookAtIKBone(Mesh.MeshData, AssetCustomData->HeadBone, CameraLocation, Mesh.IKWeight,
					                  AssetCustomData->HeadIKStiffness, DeltaTime,
					                  OffsetTransform);
				}

				// todo: Multithreading stuff. Need to look up multithreading
				// Ah so switching groups are locked, editted and then unlocked
				// Seems to be two different tiers of solving, determined by switch groups
				if (CameraDistance < RadiusOfHighQualitySolving)
				{
					if (Mesh.CurrentUpdateGroupIndex == Mesh.DefaultUpdateGroupIndex)
					{
						CriticalSection.Lock();
						SwitchingGroups.Add(Mesh.MeshData, true);
						CriticalSection.Unlock();
					}
				}
				// 
				else
				{
					if (Mesh.CurrentUpdateGroupIndex == QualityGroupIndex)
					{
						CriticalSection.Lock();
						SwitchingGroups.Add(Mesh.MeshData, false);
						CriticalSection.Unlock();
					}
				}

				Mesh.DeltaTimeAccumulator = 0;
			}
		}
	}, EParallelForFlags::BackgroundPriority);
}

/*
 * Get a random mesh and material combination, and store it into a FTurboSequence_MeshSpawnData_Lf
 */
void ATurboSequence_Demo_Lf::GetRandomMeshSpawnData(FTurboSequence_MeshSpawnData_Lf& Data,
                                                    const FDemoCustomizationContainer_Lf& CategorizedRootData,
                                                    const TMap<FName, FDemoCustomizationContainer_Lf>&
                                                    CategorizeCustomizableData,
                                                    const TObjectPtr<UTurboSequence_FootprintAsset_Lf> FootprintAsset)
{
	Data = FTurboSequence_MeshSpawnData_Lf();
	// Find valid random root mesh
	if (int32 RandomRootMesh = FMath::RandRange(0, CategorizedRootData.CategorizedData.Num() - 1);
		CategorizedRootData.CategorizedData.IsValidIndex(RandomRootMesh))
	{
		// Set spawn data
		Data.RootMotionMesh.Mesh = CategorizedRootData.CategorizedData[RandomRootMesh].Asset;
		Data.RootMotionMesh.FootprintAsset = FootprintAsset;
		// If there is any materials, set those too
		if (CategorizedRootData.CategorizedData[RandomRootMesh].Materials.Num())
		{
			TArray<FName> Names;
			CategorizedRootData.CategorizedData[RandomRootMesh].Materials.GetKeys(Names);
			int32 RandomRootMaterial = FMath::RandRange(
				0, CategorizedRootData.CategorizedData[RandomRootMesh].Materials.Num() - 1);
			Data.RootMotionMesh.OverrideMaterials = CategorizedRootData.CategorizedData[RandomRootMesh].Materials[
				Names[RandomRootMaterial]].Materials;
		}
	}
	// For each customisable data (spawn data)
	for (const TTuple<FName, FDemoCustomizationContainer_Lf>& Customizable : CategorizeCustomizableData)
	{
		// If spawn data exists
		if (Customizable.Value.CategorizedData.Num())
		{
			// Get a random spawn data
			int32 RandomMesh = FMath::RandRange(0, Customizable.Value.CategorizedData.Num() - 1);

			// Create mesh data
			FTurboSequence_MeshMetaData_Lf MeshData = FTurboSequence_MeshMetaData_Lf();

			// Populate MeshData with the turbo squid mesh
			MeshData.Mesh = Customizable.Value.CategorizedData[RandomMesh].Asset;
			MeshData.FootprintAsset = FootprintAsset; // todo: Still not too sure what a footprint asset is?

			// If spawn data has a material
			if (Customizable.Value.CategorizedData[RandomMesh].Materials.Num())
			{
				// Populate Names with keys
				TArray<FName> Names;
				Customizable.Value.CategorizedData[RandomMesh].Materials.GetKeys(Names);

				// Randomise the overriding material using the collection stored in spawn data materials
				int32 RandomMaterial = FMath::RandRange(
					0, Customizable.Value.CategorizedData[RandomMesh].Materials.Num() - 1);
				MeshData.OverrideMaterials = Customizable.Value.CategorizedData[RandomMesh].Materials[Names[
					RandomMaterial]].Materials;
			}

			// Add MeshData to CustomizableMeshes
			Data.CustomizableMeshes.Add(MeshData);
		}
	}
}

void ATurboSequence_Demo_Lf::SolveLookAtIKBone(const FTurboSequence_MinimalMeshData_Lf& MeshData, const FName& BoneName,
                                               const FVector& TargetLocation, float Weight, float OffsetWeight,
                                               float AnimationDeltaTime,
                                               const FTransform& StartOffsetTransform)
{
	FTransform IKTransform;
	ATurboSequence_Manager_Lf::GetIKTransform_Concurrent(IKTransform, MeshData, BoneName, AnimationDeltaTime,
	                                                     EBoneSpaces::WorldSpace);

	FTransform ReferencePoseTransform;
	ATurboSequence_Manager_Lf::GetReferencePoseTransform_Concurrent(ReferencePoseTransform, MeshData, BoneName,
	                                                                ETurboSequence_TransformSpace_Lf::ComponentSpace);
	ReferencePoseTransform *= StartOffsetTransform;

	const FQuat& WantedRotation = FRotationMatrix::MakeFromX(TargetLocation - IKTransform.GetLocation()).ToQuat();
	IKTransform.SetRotation(FQuat::Slerp(IKTransform.GetRotation(),
	                                     WantedRotation * ReferencePoseTransform.GetRotation(),
	                                     Weight * OffsetWeight));


	ATurboSequence_Manager_Lf::SetIKTransform_Concurrent(MeshData, BoneName, IKTransform, EBoneSpaces::WorldSpace);
}

void ATurboSequence_Demo_Lf::SolveBlueprintDemoMeshesInCpp(const TArray<FTurboSequence_MinimalMeshData_Lf>& MeshData,
                                                           TArray<FVector4>& AnimData, UAnimSequence* AnimA,
                                                           UAnimSequence* AnimB,
                                                           const FTurboSequence_AnimPlaySettings_Lf& AnimSettingsA,
                                                           const FTurboSequence_AnimPlaySettings_Lf& AnimSettingsB,
                                                           const float DeltaTime, const bool bMultiThreaded)
{
	auto RunnerFunction = [&](const FTurboSequence_MinimalMeshData_Lf& MeshID, int32 Index)
	{
		FVector4& AnimDataOut = AnimData[Index];

		//AnimDataOut = AnimationData;

		AnimDataOut.X -= DeltaTime;
		if (AnimDataOut.X < 0)
		{
			AnimDataOut.X = FMath::RandRange(3.0f, 7.0f);

			if (FMath::RandBool())
			{
				ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(MeshID, AnimA, AnimSettingsA);
				ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(MeshID, AnimB, AnimSettingsB);
			}
			else
			{
				ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(MeshID, AnimB, AnimSettingsA);
				ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(MeshID, AnimA, AnimSettingsB);
			}
		}

		AnimDataOut.Y -= DeltaTime;
		if (AnimDataOut.Y < 0)
		{
			AnimDataOut.Y = FMath::RandRange(3.0f, 7.0f);
			AnimDataOut.Z = FMath::RandRange(-180.0, 180.0);
		}
		FTransform RanRotationTransform = ATurboSequence_Manager_Lf::GetMeshWorldSpaceTransform_Concurrent(MeshID);
		const FRotator CurrentRotation = RanRotationTransform.GetRotation().Rotator();
		RanRotationTransform.SetRotation(FMath::RInterpConstantTo(CurrentRotation,
		                                                          FRotator(CurrentRotation.Pitch, AnimDataOut.Z,
		                                                                   CurrentRotation.Roll), DeltaTime,
		                                                          3).Quaternion());

		ATurboSequence_Manager_Lf::SetMeshWorldSpaceLocationRotationScale_Concurrent(
			MeshID, RanRotationTransform.GetLocation(), RanRotationTransform.GetRotation(),
			RanRotationTransform.GetScale3D());

		ATurboSequence_Manager_Lf::MoveMeshWithRootMotion_Concurrent(MeshID, DeltaTime, true, false);
	};

	const int32 NumMeshes = FMath::Min(MeshData.Num(), AnimData.Num());
	if (!NumMeshes)
	{
		return;
	}

	if (bMultiThreaded)
	{
		int16 NumThreads = FTurboSequence_Helper_Lf::NumCPUThreads() - GET1_NUMBER;
		int32 NumMeshesPerThread_Background = FMath::CeilToInt(
			static_cast<float>(NumMeshes) / static_cast<float>(NumThreads));
		ParallelFor(NumThreads, [&](int32 ThreadsIndex)
		{
			const int32 MeshBaseIndex = ThreadsIndex * NumMeshesPerThread_Background;
			const int32 MeshBaseNum = MeshBaseIndex + NumMeshesPerThread_Background;

			for (int32 Index = MeshBaseIndex; Index < MeshBaseNum; ++Index)
			{
				if (Index >= NumMeshes)
				{
					break;
				}

				RunnerFunction(MeshData[Index], Index);
			}
		}, EParallelForFlags::BackgroundPriority);
	}
	else
	{
		for (int32 Index = 0; Index < NumMeshes; ++Index)
		{
			RunnerFunction(MeshData[Index], Index);
		}
	}
}
