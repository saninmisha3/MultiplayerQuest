// Fill out your copyright notice in the Description page of Project Settings.

#include "World/MovableActor.h"

#include "Components/ArrowComponent.h"
#include "Components/TransporterComponent.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	/*StartPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("StartPoint"));
	StartPoint->SetRelativeLocation(FVector(0.f));
	StartPoint->SetupAttachment(GetRootComponent());*/
	
	EndPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("EndPoint"));
	EndPoint->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	EndPoint->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetupAttachment(GetRootComponent());

	TransporterComponent = CreateDefaultSubobject<UTransporterComponent>(TEXT("TransporterComponent"));
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);

	if (TransporterComponent)
	{
		TransporterComponent->SetPoints(GetActorLocation(), EndPoint->GetComponentLocation());
	}
}

