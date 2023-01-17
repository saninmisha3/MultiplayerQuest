// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyHolder.generated.h"

class ACollectableKey;

UCLASS()
class MULTIPLAYERADVENTURE_API AKeyHolder : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> KeyMeshComponent;
	
public:	
	AKeyHolder();
	virtual void BeginPlay() override;

	void ActivateKeyHolder(const ACollectableKey* ActivatedKey);

};
