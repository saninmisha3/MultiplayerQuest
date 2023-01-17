// Fill out your copyright notice in the Description page of Project Settings.

#include "World/CollectableKey.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "World/KeyHolder.h"

ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetupAttachment(GetRootComponent());

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetIsReplicated(true);
	CapsuleComponent->SetCapsuleHalfHeight(150.f);
	CapsuleComponent->SetCapsuleRadius(100.f);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CapsuleComponent->SetupAttachment(GetRootComponent());

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetupAttachment(GetRootComponent());
}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);

	if (CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleOnBeginOverlap);
	}
}

void ACollectableKey::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsCollected && MeshComponent)
	{
		MeshComponent->AddRelativeRotation(FRotator(0.f, RotationSpeed, 0.f));
	}
}

void ACollectableKey::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (!bIsCollected && OtherActor->IsA<ACharacter>())
		{
			bIsCollected = true;
			OnRep_IsCollected();
		}
	}
}

void ACollectableKey::OnRep_IsCollected()
{
	if (MeshComponent)
	{
		MeshComponent->SetVisibility(!bIsCollected);
	}

	if (bIsCollected)
	{
		if (KeyHolder)
		{
			KeyHolder->ActivateKeyHolder(this);
		}
		
		if (AudioComponent)
		{
			AudioComponent->Play();
		}
	}

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("EUD::OnRep_IsCollected called from the server!"));
		OnCollected.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("EUD::OnRep_IsCollected called from the client!"));
	}
}

void ACollectableKey::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectableKey, bIsCollected);
}
