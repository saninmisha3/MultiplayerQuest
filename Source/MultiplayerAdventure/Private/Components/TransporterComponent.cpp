// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TransporterComponent.h"

#include "World/CollectableKey.h"
#include "World/PressureActor.h"

UTransporterComponent::UTransporterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UTransporterComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bIsOwnerTriggeredActor)
	{
		TriggerActors.Add(GetOwner());
	}
	
	for (AActor* TriggerActor : TriggerActors)
	{
		if (APressureActor* PressureActor = Cast<APressureActor>(TriggerActor))
		{
			PressureActor->OnActivationStateChanged.AddDynamic(this, &ThisClass::OnTriggerActorActivationChanged);
			continue;
		}

		if (ACollectableKey* CollectableKey = Cast<ACollectableKey>(TriggerActor))
		{
			CollectableKey->OnCollected.AddDynamic(this, &ThisClass::OnTriggerActorActivationChanged);
			continue;
		}
	}
}

void UTransporterComponent::SetPoints(const FVector InStartLocation, const FVector InEndLocation)
{
	StartLocation = InStartLocation;
	EndLocation = InEndLocation;

	bArePointsSet = true;
}

void UTransporterComponent::OnTriggerActorActivationChanged(const bool bIsActivated)
{
	bIsActivated ? ActivatedTriggerCount++ : ActivatedTriggerCount--;
	UE_LOG(LogTemp, Display, TEXT("EUD::Transporter %s"), bIsActivated ? TEXT("Activated") : TEXT("Deactivated"));

	if (bArePointsSet)
	{
		const bool bNewAllTriggered = ActivatedTriggerCount >= TriggerActors.Num();
		if (bAllTriggerActorsTriggered != bNewAllTriggered)
		{
			bAllTriggerActorsTriggered = bNewAllTriggered;
			
			const UWorld* World = GetWorld();
			if (World && !World->GetTimerManager().IsTimerActive(Timer_UpdateLocation))
			{
				World->GetTimerManager().SetTimer(Timer_UpdateLocation, this, &UTransporterComponent::HandleUpdateLocation, World->GetDeltaSeconds(), true);
			}
		}
	}
}

void UTransporterComponent::HandleUpdateLocation()
{
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	const float Speed = FVector::Distance(StartLocation, EndLocation) / MoveTime;
	const FVector TargetLocation = bAllTriggerActorsTriggered ? EndLocation : StartLocation;

	const UWorld* World = GetWorld();
	if (!World || CurrentLocation.Equals(TargetLocation))
	{
		World->GetTimerManager().ClearTimer(Timer_UpdateLocation);
		return;
	}

	const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, World->GetDeltaSeconds(), Speed);
	GetOwner()->SetActorLocation(NewLocation);
}
