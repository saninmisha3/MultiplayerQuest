// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableKey.generated.h"

class AKeyHolder;
class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const bool, bIsCollected);

UCLASS()
class MULTIPLAYERADVENTURE_API ACollectableKey : public AActor
{
	GENERATED_BODY()

public:
	FOnStateChanged OnCollected;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, Category = "CollectableKey")
	float RotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "CollectableKey")
	TObjectPtr<AKeyHolder> KeyHolder;
	
	UPROPERTY(ReplicatedUsing = "OnRep_IsCollected")
	bool bIsCollected = false;

public:	
	ACollectableKey();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE UStaticMesh* GetStaticMesh() const { return MeshComponent ? MeshComponent->GetStaticMesh() : nullptr; }

protected:
	UFUNCTION()
	void OnRep_IsCollected();

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
