// Fill out your copyright notice in the Description page of Project Settings.


#include "ISM_Components.h"

void UISM_Components::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<ISM_Map>();
	ECSWorld.component<ISM_ControllerRef>();
	ECSWorld.component<ISM_Hash>();
	ECSWorld.component<ISM_Index>();
	ECSWorld.component<ISM_AddInstance>();
	ECSWorld.component<ISM_AnimationMap>();
}
