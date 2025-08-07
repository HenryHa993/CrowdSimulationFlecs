// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ISMController.generated.h"

/* Created for each unique instance of mesh and material. In this demo, it is one.*/
UCLASS()
class CROWDSIMULATION_API AISMController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AISMController();

public:
	void Initialize(UStaticMesh* StaticMesh, UMaterialInterface* Material) const;

	void CreateOrExpandTransformArray();

	int32 GetInstanceCount() const;

	int32 AddInstance(FVector Location);

	int32 AddInstance();

	void SetTransform(int32 InstanceIndex, const FTransform& Transform);

	void BatchUpdateTransforms();

	void ClearInstances();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

private:
	TArray<FTransform> Transforms;
	TQueue<int> IndexPool;
};
