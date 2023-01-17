// Fill out your copyright notice in the Description page of Project Settings.

#include "World/WinVolume.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

AWinVolume::AWinVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WanAreaBox"));
	SetRootComponent(WinAreaBox);
}

void AWinVolume::BeginPlay()
{
	Super::BeginPlay();

	if (WinAreaBox)
	{
		WinAreaBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleOnBeginOverlap);
		WinAreaBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleOnEndOverlap);
	}
}

void AWinVolume::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (OtherActor->IsA<ACharacter>())
		{
			CharactersOverlapping++;

			if (CharactersOverlapping == WinThreshold)
			{
				SetWinCondition(true);
			}
		}
	}
}

void AWinVolume::HandleOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		if (OtherActor->IsA<ACharacter>())
		{
			CharactersOverlapping--;
		}
	}
}

void AWinVolume::SetWinCondition(const bool bIsWin)
{
	if (bIsWin)
	{
		bWinCondition = true;
		Multicast_Win();
	}
}

void AWinVolume::Multicast_Win_Implementation()
{
	OnWinCondition.Broadcast();
}
