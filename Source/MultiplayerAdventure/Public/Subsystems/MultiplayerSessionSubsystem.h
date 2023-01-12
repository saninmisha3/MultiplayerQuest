// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionSubsystem.generated.h"

UCLASS()
class MULTIPLAYERADVENTURE_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	IOnlineSessionPtr SessionInterface;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	///////////////////////////////
	// Server API
	///////////////////////////////
	UFUNCTION(BlueprintCallable)
	void CreateServer(const FString& ServerName);

	UFUNCTION(BlueprintCallable)
	void JoinServer(const FString& ServerName);

private:
	void PrintString(const FString& Message) const;

	///////////////////////////////
	// Session Interface Delegate Handles
	///////////////////////////////
	void HandleOnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful);
};
