// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TransporterComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERADVENTURE_API UTransporterComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Transporter")
	float MoveTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "Transporter")
	bool bIsOwnerTriggeredActor = false;

	UPROPERTY(EditAnywhere, Category = "Transporter")
	TArray<TObjectPtr<AActor>> TriggerActors;
	
	FVector StartLocation = FVector::Zero();
	FVector EndLocation = FVector::Zero();
	
	bool bArePointsSet = false;
	bool bAllTriggerActorsTriggered = false;
	
	int32 ActivatedTriggerCount = 0;
	
public:	
	UTransporterComponent();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetPoints(const FVector InStartLocation, const FVector InEndLocation);

	// Callbacks
	UFUNCTION()
	void OnTriggerActorActivationChanged(const bool bIsActivated);

	FTimerHandle Timer_UpdateLocation;
	void HandleUpdateLocation();
};
