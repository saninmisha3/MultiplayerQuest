// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

class UTransporterComponent;
class UArrowComponent;

UCLASS()
class MULTIPLAYERADVENTURE_API AMovableActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USceneComponent> SceneComponent;

	/*UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UArrowComponent> StartPoint;*/

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UArrowComponent> EndPoint;

	UPROPERTY(VisibleAnywhere, Category= "Component")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UTransporterComponent> TransporterComponent;
	
public:	
	AMovableActor();
	
	virtual void BeginPlay() override;


};
