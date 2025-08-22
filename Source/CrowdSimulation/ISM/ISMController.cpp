// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMController.h"

#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AISMController::AISMController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedStaticMeshComponent");
	SetRootComponent(InstancedStaticMeshComponent);
}

void AISMController::Initialize(UStaticMesh* StaticMesh, UMaterialInterface* Material) const
{
	InstancedStaticMeshComponent->SetStaticMesh(StaticMesh);
	InstancedStaticMeshComponent->SetMaterial(0, Material);

	InstancedStaticMeshComponent->NumCustomDataFloats = 2;
	InstancedStaticMeshComponent->bDisableCollision = true;
	InstancedStaticMeshComponent->bVisibleInRayTracing = false;
	// Todo: Testing some settings
	// InstancedStaticMeshComponent->SetCastShadow(false);
}

void AISMController::CreateOrExpandTransformArray()
{
	if(GetInstanceCount() != Transforms.Num())
	{
		Transforms.AddUninitialized(GetInstanceCount() - Transforms.Num());
		for(int i = 0; i < Transforms.Num(); i++)
		{
			Transforms[i] = FTransform::Identity;
		}
	}
}

int32 AISMController::GetInstanceCount() const
{
	return InstancedStaticMeshComponent->GetInstanceCount();
}

int32 AISMController::AddInstance(FVector Location)
{
	int32 instanceIndex;
	if(IndexPool.IsEmpty())
	{
		FTransform transform{Location};
		instanceIndex = InstancedStaticMeshComponent->AddInstance(transform);
	}
	else
	{
		// Out item instance index
		IndexPool.Dequeue(instanceIndex);
		// todo: re-enable instance
	}
	return instanceIndex;
}

int32 AISMController::AddInstance()
{
	return AddInstance(FVector::ZeroVector);
}

void AISMController::SetTransform(int32 InstanceIndex, const FTransform& Transform)
{
	Transforms[InstanceIndex] = Transform;
}

void AISMController::BatchUpdateTransforms()
{
	if(!Transforms.IsEmpty())
	{
		InstancedStaticMeshComponent->BatchUpdateInstancesTransforms(0, Transforms, true, true);
	}
}

// Used to despawn all instances in GUI
void AISMController::ClearInstances()
{
	InstancedStaticMeshComponent->ClearInstances();
	Transforms.Empty();
}

// Since custom data indexes 2 and 3 are reserved for BeginFrame and EndFrame, we could simply change it on each instance to animate them
void AISMController::SetAnimation(int32 InstanceIndex, float StartFrame, float EndFrame)
{
	UE_LOG(LogTemp, Warning, TEXT("Switching animation!"));
	InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 0, StartFrame, true);
	InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 1, EndFrame, true);
}

