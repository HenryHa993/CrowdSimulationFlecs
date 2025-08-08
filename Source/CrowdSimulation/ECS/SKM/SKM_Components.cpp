// Fill out your copyright notice in the Description page of Project Settings.


#include "SKM_Components.h"

void USKM_Components::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<SKM_AddInstance>();
	ECSWorld.component<SKM_ActorRef>();
	ECSWorld.component<SKM_SkeletalMeshRef>();
}
