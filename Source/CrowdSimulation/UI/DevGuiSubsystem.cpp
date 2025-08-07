// Fill out your copyright notice in the Description page of Project Settings.


#include "DevGuiSubsystem.h"
#include <imgui.h>

#include "ImGuiModule.h"
#include "UnrealFlecsSubsystem.h"
#include "CrowdSimulation/ECS/TS/TS_Components.h"
#include "Kismet/GameplayStatics.h"

UE_DISABLE_OPTIMIZATION


void UDevGuiSubsystem::ActorDebugger(bool& bActorDebuggerOpened)
{
	static bool bIsPickingActor = false;
	static TWeakObjectPtr<AActor> PickedActor = nullptr;

	if(!bActorDebuggerOpened)
	{
		return;
	}
	
	ImGui::Begin("Actor Debugger");

	if(ImGui::Button(bIsPickingActor ? "Stop Picking" : "Start Picking"))
	{
		bIsPickingActor = !bIsPickingActor;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	ULocalPlayer* LP = World ? World->GetFirstLocalPlayerFromController() : nullptr;
	if(bIsPickingActor)
	{
		if (LP && LP->ViewportClient)
		{
			// get the projection data
			FSceneViewProjectionData ProjectionData;
			if (LP->GetProjectionData(LP->ViewportClient->Viewport, ProjectionData))
			{
				FMatrix const InvViewProjMatrix = ProjectionData.ComputeViewProjectionMatrix().InverseFast();
				ImVec2 ScreenPosImGui = ImGui::GetMousePos();
				FVector2D ScreenPos = {ScreenPosImGui.x, ScreenPosImGui.y};
				FVector WorldPosition, WorldDirection;
				FSceneView::DeprojectScreenToWorld(ScreenPos, ProjectionData.GetConstrainedViewRect(), InvViewProjMatrix, WorldPosition, WorldDirection);

				FCollisionQueryParams Params("DevGuiActorPickerTrace", SCENE_QUERY_STAT_ONLY(UDevGuiSubsystem), true);
				Params.bReturnPhysicalMaterial = false;
				Params.bReturnFaceIndex = false;

				FCollisionObjectQueryParams ObjectParams(
					ECC_TO_BITFIELD(ECC_WorldStatic)
					| ECC_TO_BITFIELD(ECC_WorldDynamic)
					| ECC_TO_BITFIELD(ECC_Pawn)
					| ECC_TO_BITFIELD(ECC_PhysicsBody));
				
				PickedActor = nullptr;
				FHitResult OutHit;
				if(World->LineTraceSingleByObjectType(
						OutHit,
						WorldPosition + WorldDirection * 100.0,
						WorldPosition + WorldDirection * 10000.0,
						ObjectParams,
						Params))
				{
					PickedActor = OutHit.GetActor();
				}
			}
		}

		if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			bIsPickingActor = false;
		}
	}

	if(AActor* Actor = PickedActor.Get())
	{
		// ImGui::BeginChild("PickedActorFrame", ImVec2(), true);
		ImGui::Text("Picked Actor: %ls", *Actor->GetName());

		Actor->ForEachComponent<UStaticMeshComponent>(true, [](UStaticMeshComponent* Mesh)
		{
			auto NameANSI = StringCast<ANSICHAR>(*Mesh->GetName());
			if(ImGui::CollapsingHeader(NameANSI.Get(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				if(auto StaticMesh = Mesh->GetStaticMesh())
				{
					ImGui::Text("Mesh name: %ls", *StaticMesh->GetName());
					ImGui::Text("Nanite Enabled: %s", StaticMesh->NaniteSettings.bEnabled ? "true" : "false");
				}

				// ImGui::PushID(NameANSI.Get());
				if(Mesh->IsSimulatingPhysics())
				{
					FString ButtonLabel = FString::Printf(TEXT("Add Vertical Force##%s"), NameANSI.Get());
					if(ImGui::Button(TCHAR_TO_UTF8(*ButtonLabel)))
					{
						Mesh->AddForce(FVector{0.0, 0.0, 50000.0}, NAME_None, true);
					}
				}
				// ImGui::PopID();
			}
		});

		// ImGui::EndChild();
	}
	else
	{
		PickedActor = nullptr;
	}
	
	ImGui::End();
}

bool UDevGuiSubsystem::TickFinal()
{
	static bool bActorDebuggerOpened = false;

	ActorDebugger(bActorDebuggerOpened);
	
	/*if(!FImGuiModule::Get().GetProperties().IsInputEnabled())
	{
		return true;
	}*/

	// 
	if(ImGui::BeginMainMenuBar())
	{
		ImGui::Text("Website Support");
		if(ImGui::MenuItem("Toggle Actor Debugger"))
		{
			bActorDebuggerOpened = !bActorDebuggerOpened;
		}
		
		ImGui::EndMainMenuBar();
	}
	return false;
}

void UDevGuiSubsystem::HelloWorldTick()
{
	static bool bShowDemo = false;
	static bool bShowHelloWorld = false;

	if(bShowHelloWorld)
	{
		ImGui::Begin("My Little Window :)");
		ImGui::Text("Hello, world !");
		ImGui::Text("We're inside: %ls", *GetName());
		FVector CameraPos = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		ImGui::Text("Camera Position: %.2f %.2f %.2f", CameraPos.X, CameraPos.Y, CameraPos.Z);
		if(ImGui::Button("Toggle Demo"))
		{
			bShowDemo = !bShowDemo;
		}
		ImGui::End();
	}

	if(bShowDemo)
	{
		ImGui::ShowDemoWindow(&bShowDemo);
	}

	/*if(!FImGuiModule::Get().GetProperties().IsInputEnabled())
	{
		return;
	}*/
	
	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu("ImGui Misc"))
		{
			if(ImGui::MenuItem("Toggle Hello World"))
			{
				bShowHelloWorld = !bShowHelloWorld;
			}
			if(ImGui::MenuItem("Toggle Demo"))
			{
				bShowDemo = !bShowDemo;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void UDevGuiSubsystem::MainMenu()
{
	if(ImGui::BeginMainMenuBar())
	{
		// Menus for changing parameters
		if(ImGui::MenuItem("Toggle Spawn Menu"))
		{
			bShowSpawnMenu = !bShowSpawnMenu;
		}
		if(ImGui::MenuItem("Toggle Rendering Menu"))
		{
			bShowRenderMenu = !bShowRenderMenu;
		}
		if(ImGui::MenuItem("Toggle AI Menu"))
		{
			bShowAIMenu = !bShowAIMenu;
		}
		if(ImGui::MenuItem("Toggle Stats"))
		{
			bShowAIMenu = !bShowAIMenu;
		}

		ImGui::EndMainMenuBar();
	}

	this->SpawnMenu(bShowSpawnMenu);
	this->RenderMenu(bShowRenderMenu);
	this->AIMenu(bShowAIMenu);
	this->Stats(bShowStats);

	ImGui::ShowDemoWindow();
}

void UDevGuiSubsystem::SpawnMenu(bool& bShow)
{
	if(!bShow)
	{
		return;
	}

	ImGui::Begin("Spawn Menu");
	ImGui::SliderInt("Number of Units", &NumUnits, 1, 10000);
	ImGui::SliderFloat("Distance between Units", &DistanceBetweenUnits, 0, 10000);

	if(ImGui::Button("Spawn Units"))
	{
		this->SpawnUnits();
	}
	if(ImGui::Button("Despawn Units"))
	{
		this->DespawnUnits();
	}
	ImGui::End();
}

void UDevGuiSubsystem::RenderMenu(bool& bShow)
{
	if(!bShow)
	{
		return;
	}
	
	ImGui::Begin("Render Menu");
	ImGui::End();
}

void UDevGuiSubsystem::AIMenu(bool& bShow)
{
	if(!bShow)
	{
		return;
	}

	ImGui::Begin("AI Menu");
	ImGui::End();
}

void UDevGuiSubsystem::Stats(bool& bShow)
{
	if(!bShow)
	{
		return;
	}

	ImGui::Begin("Stats");
	ImGui::End();
}

// todo: Spawning system currently only works for flecs system
void UDevGuiSubsystem::SpawnUnits()
{
	// Get world
	flecs::world* ecs = GetGameInstance()->GetSubsystem<UUnrealFlecsSubsystem>()->GetECSWorld();
	// Get the prefab entity
	// Should I store everything I need in the prefab?
	flecs::entity unitPrefab = ecs->lookup("TS Prefab");
	
	// Square root
	int sqrtNumUnits = FMath::RoundToInt(FMath::Sqrt(static_cast<float>(NumUnits)));

	// Spawn
	for(float x = 0; x < sqrtNumUnits * DistanceBetweenUnits; x += DistanceBetweenUnits)
	{
		for(float y = 0; y < sqrtNumUnits * DistanceBetweenUnits; y += DistanceBetweenUnits)
		{
			FTransform unitTransform{FVector{x, y, 0}};
			ecs->entity()
				.set<TS_AddInstance>({GetWorld(), unitPrefab, unitTransform});
		}
	}
}

void UDevGuiSubsystem::DespawnUnits()
{
	// I would have to generate a query and then add the delete component onto all entities I think?

	// Is a TS_Prefab

	// Or go through each and every entity, check if it is a prefab and delete.

	// Or turn enable a system
	flecs::world* ecs = GetGameInstance()->GetSubsystem<UUnrealFlecsSubsystem>()->GetECSWorld();

	// I think this is fine for now
	flecs::entity despawnPipeline = ecs->pipeline()
	.with(flecs::System)
	.with<OnDespawn>()
	.build();

	ecs->run_pipeline(despawnPipeline);
}

void UDevGuiSubsystem::Tick(float DeltaTime)
{
	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		/*this->TickFinal();
		this->HelloWorldTick();*/
		MainMenu();
	}
}

TStatId UDevGuiSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDevGuiSubsystem, STATGROUP_Tickables);
}

UE_ENABLE_OPTIMIZATION
