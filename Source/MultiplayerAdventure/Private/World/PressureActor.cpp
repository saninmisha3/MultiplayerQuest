// Fill out your copyright notice in the Description page of Project Settings.

#include "World/PressureActor.h"
#include "Components/SphereComponent.h"

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
}

void APressureActor::HandleOnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogPressedActor, Display, TEXT("[%s] Activated by %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));
}

void APressureActor::HandleOnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogPressedActor, Display, TEXT("[%s] Deactivated by %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));
}
