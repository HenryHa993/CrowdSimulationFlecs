// Fill out your copyright notice in the Description page of Project Settings.


#include "ST_Components.h"

void UST_Components::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<ST_AddInstance>();
	ECSWorld.component<ST_ActorRef>();
}
