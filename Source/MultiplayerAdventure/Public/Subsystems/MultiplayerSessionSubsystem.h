// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionStateChanged, bool, bWasSuccessful);

UCLASS()
class MULTIPLAYERADVENTURE_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSessionStateChanged OnServerCreateCompleted;

	UPROPERTY(BlueprintAssignable)
	FSessionStateChanged OnJoinServerCompleted;
	
protected:
	IOnlineSessionPtr SessionInterface;
	
	FString TravelMapPath = FString();
	
private:
	bool bRecreateServer = false;
	FString RecreateServerName;

	FString ServerNameToFind;
	FName OurSessionName = FName("Multiplayer Adventure Session Name");

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	///////////////////////////////
	// Server API
	///////////////////////////////
	UFUNCTION(BlueprintCallable)
	void CreateServer(const FString& ServerName, const FString& InTravelMapPath);

	UFUNCTION(BlueprintCallable)
	void JoinServer(const FString& ServerName);

private:
	void PrintString(const FString& Message, const bool bIsError = false) const;

	///////////////////////////////
	// Session Interface Delegate Handles
	///////////////////////////////
	void HandleOnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful);
	void HandleOnDestroySessionComplete(const FName SessionName, const bool bWasSuccessful);
	void HandleOnFindSessionsComplete(const bool bWasSuccessful);
	void HandleOnJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult);
};
