// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Components.h"

void UTS_Components::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<TS_MeshUpdateContext>();
	ECSWorld.component<TS_AddInstance>();
	ECSWorld.component<TS_Mesh>();
	ECSWorld.component<TS_Animation>();
	ECSWorld.component<TS_Animations>();
}
