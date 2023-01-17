// Fill out your copyright notice in the Description page of Project Settings.

#include "World/PressureActor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/TransporterComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPressedActor, Log, All);

APressureActor::APressureActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(MeshComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &APressureActor::HandleOnSphereCollisionBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &APressureActor::HandleOnSphereCollisionEndOverlap);
	SphereCollision->SetupAttachment(GetRootComponent());

	TransporterComponent = CreateDefaultSubobject<UTransporterComponent>(TEXT("TransporterComponent"));
}

void APressureActor::BeginPlay()
{
	Super::BeginPlay();

	if (TransporterComponent)
	{
		TransporterComponent->SetPoints(GetActorLocation(), GetActorLocation() + TriggerOffset);
	}
}

void APressureActor::HandleOnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActivated && OtherActor->IsA<ACharacter>())
	{
		bIsActivated = true;
		OnActivationStateChanged.Broadcast(bIsActivated);
	}
}

void APressureActor::HandleOnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsActivated && OtherActor->IsA<ACharacter>())
	{
		bIsActivated = false;
		OnActivationStateChanged.Broadcast(bIsActivated);
	}
}
