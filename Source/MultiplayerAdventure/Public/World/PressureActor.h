// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressureActor.generated.h"

class UTransporterComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivationStateChanged, const bool, bIsActivated);

UCLASS()
class MULTIPLAYERADVENTURE_API APressureActor : public AActor
{
	GENERATED_BODY()

public:
	FOnActivationStateChanged OnActivationStateChanged;
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UTransporterComponent> TransporterComponent;

	UPROPERTY(EditAnywhere, Category = "Pressure Actor")
	FVector TriggerOffset = FVector::Zero();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pressure Properties")
	bool bIsActivated = false;
	
public:	
	APressureActor();
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	void HandleOnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleOnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
