// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinVolume.generated.h"

class UBoxComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWinCondition);

UCLASS()
class MULTIPLAYERADVENTURE_API AWinVolume : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnWinCondition OnWinCondition;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> WinAreaBox;

	UPROPERTY(EditAnywhere, Category = "WinVolume")
	int32 WinThreshold = 2;

	bool bWinCondition = false;
	uint32 CharactersOverlapping = 0;
	
public:	
	AWinVolume();
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void HandleOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Win();
	
	void SetWinCondition(const bool bIsWin);

};
