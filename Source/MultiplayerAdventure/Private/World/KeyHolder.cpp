// Fill out your copyright notice in the Description page of Project Settings.

#include "World/KeyHolder.h"

#include "World/CollectableKey.h"

AKeyHolder::AKeyHolder()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BaseMeshComponent->SetupAttachment(GetRootComponent());

	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	KeyMeshComponent->SetupAttachment(GetRootComponent());
}

void AKeyHolder::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);

	if (KeyMeshComponent)
	{
		KeyMeshComponent->SetVisibility(false);
	}
}

void AKeyHolder::ActivateKeyHolder(const ACollectableKey* ActivatedKey)
{
	if (KeyMeshComponent)
	{
		KeyMeshComponent->SetStaticMesh(ActivatedKey->GetStaticMesh());
		KeyMeshComponent->SetVisibility(true);
	}
}

