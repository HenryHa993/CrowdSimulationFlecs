// Fill out your copyright notice in the Description page of Project Settings.


#include "DevGuiSubsystem.h"
#include <imgui.h>

#include "ImGuiModule.h"
#include "UnrealFlecsSubsystem.h"
#include "CrowdSimulation/ECS/ISM/ISM_Components.h"
#include "CrowdSimulation/ECS/SKM/SKM_Components.h"
#include "CrowdSimulation/ECS/TS/TS_Components.h"
#include "Kismet/GameplayStatics.h"

UE_DISABLE_OPTIMIZATION

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
			bShowStats = !bShowStats;
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
	ImGui::Combo("Rendering Method", &SelectedRenderingItem, RenderingItems, IM_ARRAYSIZE(RenderingItems));
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
	if(ImGui::CollapsingHeader("Frame Timings"))
	{
		ImGui::Text("Frame Time: %.3f ms", FrameTime * 1000);
		
		// Frame time line plot
		FrameTimes[FrameTimesOffset] = FrameTime * 1000.0f;
		FrameTimesOffset = (FrameTimesOffset + 1) % IM_ARRAYSIZE(FrameTimes);

		// ImGui::PlotLines("Frame Time (ms)", FrameTimes, IM_ARRAYSIZE(FrameTimes), FrameTimesOffset, nullptr, 0.0f, 50.0f, ImVec2(0,80));
		ImGui::PlotHistogram(
			"Frame Time (ms)",
			FrameTimes,
			IM_ARRAYSIZE(FrameTimes),
			FrameTimesOffset,            
			nullptr,                     
			0.0f,                        
			50.0f,
			ImVec2(0, 80)
		);
		ImGui::Text("FPS: %.1f", FramesPerSecond);
		FPSs[FPSsOffset] = FramesPerSecond;
		FPSsOffset = (FPSsOffset + 1) % IM_ARRAYSIZE(FPSs);
		ImGui::PlotLines("FPS", FPSs, IM_ARRAYSIZE(FPSs), FPSsOffset, nullptr, 0.0f, 50.0f, ImVec2(0,80));
		
	}
	// This can be on hold for now
	if(ImGui::CollapsingHeader("CPU & GPU Timings"))
	{
		
	}
	if(ImGui::CollapsingHeader("Memory Usage"))
	{
		
	}
	ImGui::End();
}

// todo: Spawning system currently only works for flecs system
void UDevGuiSubsystem::SpawnUnits()
{
	flecs::world* ecs = GetGameInstance()->GetSubsystem<UUnrealFlecsSubsystem>()->GetECSWorld();
	flecs::entity unitPrefab = ecs->lookup("Unit Prefab");
	
	int sqrtNumUnits = FMath::RoundToInt(FMath::Sqrt(static_cast<float>(NumUnits)));

	// Spawn units in a grid based on values
	for(float x = 0; x < sqrtNumUnits * DistanceBetweenUnits; x += DistanceBetweenUnits)
	{
		for(float y = 0; y < sqrtNumUnits * DistanceBetweenUnits; y += DistanceBetweenUnits)
		{
			FTransform unitTransform{FVector{x, y, 0}};

			// todo: Separate these out into functions
			// Separate logic based on different modes of rendering
			switch (SelectedRenderingItem)
			{
				// TurboSequence
				case 0:
					{
						ecs->entity()
					   .set<TS_AddInstance>({unitPrefab, unitTransform});
						break;
					}
				// ISM + Vertex Animations
				case 1:
					{
						AISMController* controller = ecs->get<ISM_ControllerRef>()->Value;
					
						ecs->entity()
						.set<ISM_AddInstance>({unitPrefab, unitTransform});
						break;
					}
				// Skeletal Mesh
				case 2:
					{
						ecs->entity()
						.set<SKM_AddInstance>({unitPrefab, unitTransform});
						break;
					}
			}
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

void UDevGuiSubsystem::UpdateStats(float deltaTime)
{
	FrameTime = deltaTime;
	FramesPerSecond = 1.0f / deltaTime;
}

UDevGuiSubsystem::UDevGuiSubsystem()
{
	RenderingItems[0] = "GPU Instancing w/ Niagara (TurboSequence)";
	RenderingItems[1] = "Instanced Static Mesh + Vertex Animations";
	RenderingItems[2] = "Skeletal Mesh";
}

void UDevGuiSubsystem::Tick(float DeltaTime)
{
	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		UpdateStats(DeltaTime);
		MainMenu();
	}
}

TStatId UDevGuiSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDevGuiSubsystem, STATGROUP_Tickables);
}

UE_ENABLE_OPTIMIZATION
