// Fill out your copyright notice in the Description page of Project Settings.


#include "DevGuiSubsystem.h"
#include <imgui.h>

#include "ImGuiModule.h"
#include "UnrealFlecsSubsystem.h"
#include "CrowdSimulation/ECS/ISM/ISM_Components.h"
#include "CrowdSimulation/ECS/SKM/SKM_Components.h"
#include "CrowdSimulation/ECS/ST/ST_Components.h"
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
		/*if(ImGui::MenuItem("Toggle Rendering Menu"))
		{
			bShowRenderMenu = !bShowRenderMenu;
		}
		if(ImGui::MenuItem("Toggle AI Menu"))
		{
			bShowAIMenu = !bShowAIMenu;
		}*/
		if(ImGui::MenuItem("Toggle Stats"))
		{
			bShowStats = !bShowStats;
		}

		ImGui::EndMainMenuBar();
	}

	this->SpawnMenu(bShowSpawnMenu);
	//this->RenderMenu(bShowRenderMenu);
	//this->AIMenu(bShowAIMenu);
	this->Stats(bShowStats);

	//ImGui::ShowDemoWindow();
}

void UDevGuiSubsystem::SpawnMenu(bool& bShow)
{
	if(!bShow)
	{
		return;
	}

	ImGui::Begin("Spawn Menu");
	ImGui::Combo("Framework", &SelectedFrameworkItem, FrameworkItems, IM_ARRAYSIZE(FrameworkItems));
	ImGui::Combo("Rendering", &SelectedRenderingItem, RenderingItems, IM_ARRAYSIZE(RenderingItems));
	ImGui::BeginGroup();
	ImGui::SliderInt("Number of Units##slider", &NumUnits, 1, 10000);
	ImGui::SameLine();
	ImGui::InputInt("Number of Units##input", &NumUnits, 1);
	ImGui::EndGroup();
	ImGui::BeginGroup();
	ImGui::SliderFloat("Distance between Units##slider", &DistanceBetweenUnits, 0, 10000);
	ImGui::SameLine();
	ImGui::InputFloat("Distance between Units##value", &DistanceBetweenUnits, .1f);
	ImGui::EndGroup();

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
	
	/*if(ImGui::CollapsingHeader("Frame Time & FPS"))
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
		
	}*/

	/*if(ImGui::CollapsingHeader("Game Thread & Draw Thread"))
	{
		ImGui::Text("Game Thread Time: %.2f ms", GameThreadTime);
		
		GameThreadTimes[GameThreadTimesOffset] = GameThreadTime;
		GameThreadTimesOffset = (GameThreadTimesOffset + 1) % IM_ARRAYSIZE(GameThreadTimes);

		// ImGui::PlotLines("Frame Time (ms)", FrameTimes, IM_ARRAYSIZE(FrameTimes), FrameTimesOffset, nullptr, 0.0f, 50.0f, ImVec2(0,80));
		ImGui::PlotHistogram(
			"Game Thread Time (ms)",
			GameThreadTimes,
			IM_ARRAYSIZE(GameThreadTimes),
			GameThreadTimesOffset,            
			nullptr,                     
			0.0f,                        
			50.0f,
			ImVec2(0, 80)
		);
		
		ImGui::Text("Render Thread Time: %.2f ms", RenderThreadTime);

		RenderThreadTimes[RenderThreadTimesOffset] = RenderThreadTime;
		RenderThreadTimesOffset = (RenderThreadTimesOffset + 1) % IM_ARRAYSIZE(RenderThreadTimes);

		// ImGui::PlotLines("Frame Time (ms)", FrameTimes, IM_ARRAYSIZE(FrameTimes), FrameTimesOffset, nullptr, 0.0f, 50.0f, ImVec2(0,80));
		ImGui::PlotHistogram(
			"Render Thread Time (ms)",
			RenderThreadTimes,
			IM_ARRAYSIZE(RenderThreadTimes),
			RenderThreadTimesOffset,
			nullptr,
			0.0f,
			50.0f,
			ImVec2(0, 80)
		);
	}*/
	/*if(ImGui::CollapsingHeader("Memory Usage"))
	{
		
	}*/
	/*if(ImGui::CollapsingHeader("Console Commands"))
	{
		if(ImGui::Button("Stat UNIT"))
		{
			APlayerController* controller = GetWorld()->GetFirstPlayerController();
			controller->ConsoleCommand(TEXT("Stat UNIT"));
		}
		if(ImGui::Button("Stat GAME"))
		{
			APlayerController* controller = GetWorld()->GetFirstPlayerController();
			controller->ConsoleCommand(TEXT("Stat GAME"));
		}	
	}*/
	// Basics
		if(ImGui::CollapsingHeader("Basic Stats"))
	{
			if(ImGui::Button("Stat FPS"))
			{
				APlayerController* controller = GetWorld()->GetFirstPlayerController();
				controller->ConsoleCommand(TEXT("Stat FPS"));
			}
			if(ImGui::Button("Stat UNIT"))
			{
				APlayerController* controller = GetWorld()->GetFirstPlayerController();
				controller->ConsoleCommand(TEXT("Stat UNIT"));
			}
			if(ImGui::Button("Stat UNITGRAPH"))
			{
				APlayerController* controller = GetWorld()->GetFirstPlayerController();
				controller->ConsoleCommand(TEXT("Stat UNITGRAPH"));
			}
			// Tick
			if(ImGui::Button("Stat GAME"))
			{
				APlayerController* controller = GetWorld()->GetFirstPlayerController();
				controller->ConsoleCommand(TEXT("Stat GAME"));
			}
			// Number of draw calls
			if(ImGui::Button("Stat SCENERENDERING"))
			{
				APlayerController* controller = GetWorld()->GetFirstPlayerController();
				controller->ConsoleCommand(TEXT("Stat SCENERENDERING"));
			}	
	}
	if(ImGui::CollapsingHeader("Profiling"))
	{
		if(ImGui::Button("MemReport"))
		{
			APlayerController* controller = GetWorld()->GetFirstPlayerController();
			controller->ConsoleCommand(TEXT("MemReport"));
		}
		ImGui::InputFloat("Bookmark Timer", &AutoBookmarkTimer, .1f);
		if(ImGui::Button("Start Auto Bookmark"))
		{
			bAutoBookmarkStarted = true;
			TRACE_BOOKMARK(TEXT("Auto Bookmark Started"));
		}
		if(ImGui::Button("MemReport"))
		{
			APlayerController* controller = GetWorld()->GetFirstPlayerController();
			controller->ConsoleCommand(TEXT("MemReport"));
		}
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
			switch (SelectedFrameworkItem)
			{
			// Index 0 represents Unreal Engine framework
			case 0:
				switch (SelectedRenderingItem)
				{
						// Skeletal Mesh
				case 0:
					{
						ecs->entity()
					   .set<ST_AddInstance>({SelectedRenderingItem, unitTransform});
						break;
					}
						// ISM + Vertex Animations
				case 1:
					{
						ecs->entity()
						.set<ST_AddInstance>({SelectedRenderingItem, unitTransform});
						break;
					}
						// Turbosequence
				case 2:
					{
						ecs->entity()
						.set<ST_AddInstance>({SelectedRenderingItem, unitTransform});
						break;
					}
				}
				break;

			// Index 1 represents the FLECS framework
			case 1:
				switch (SelectedRenderingItem)
				{
						// Skeletal mesh actors
				case 0:
					{
						ecs->entity()
						.set<SKM_AddInstance>({unitPrefab, unitTransform});
						break;
					}
						// ISM + Vertex Animations
				case 1:
					{
						//AISMController* controller = ecs->get<ISM_ControllerRef>()->Value;
							
						ecs->entity()
						.set<ISM_AddInstance>({unitPrefab, unitTransform});
						break;
					}
						// Turbosequence actors
				case 2:
					{
						ecs->entity()
					   .set<TS_AddInstance>({unitPrefab, unitTransform});
						break;
					}
				}
				break;
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

void UDevGuiSubsystem::UpdateStats(float DeltaTime)
{
	FrameTime = DeltaTime;
	FramesPerSecond = 1.0f / DeltaTime;

	GameThreadTime = FPlatformTime::ToMilliseconds(GGameThreadTime);
	RenderThreadTime = FPlatformTime::ToMilliseconds(GRenderThreadTime);
}

UDevGuiSubsystem::UDevGuiSubsystem()
{
	RenderingItems[0] = "Skeletal Mesh";
	RenderingItems[1] = "Instanced Static Mesh + Vertex Animations";
	RenderingItems[2] = "Niagara GPU Instancing (TurboSequence)";

	FrameworkItems[0] = "Unreal Engine + State Trees";
	FrameworkItems[1] = "FLECS";
}

void UDevGuiSubsystem::Tick(float DeltaTime)
{
	const ImGui::FScopedContext ScopedContext;
	if (ScopedContext)
	{
		//UpdateStats(DeltaTime);
		MainMenu();
		if(bAutoBookmarkStarted)
		{
			ElapsedAutoBookmarkTime += DeltaTime;
			if(ElapsedAutoBookmarkTime > AutoBookmarkTimer)
			{
				bAutoBookmarkStarted = false;
				TRACE_BOOKMARK(TEXT("Auto Bookmark Ended"));
				ElapsedAutoBookmarkTime = 0.f;
			}
		}
	}
}

TStatId UDevGuiSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDevGuiSubsystem, STATGROUP_Tickables);
}

UE_ENABLE_OPTIMIZATION
