// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		const FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::HandleOnCreateSessionComplete);

			PrintString("Session Interface is valid!");
		}
	}
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMultiplayerSessionSubsystem::CreateServer(const FString& ServerName)
{
	PrintString("Creating Server ...");
	
	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!");
		return;
	}

	const FName SessionName = FName("Multiplayer Adventure Session Name");

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true; // Is match is public
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	const bool bIsLAN = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	SessionSettings.bIsLANMatch = bIsLAN; 
	
	SessionInterface->CreateSession(0, SessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::JoinServer(const FString& ServerName)
{
	PrintString("Joining Server ...");
}

void UMultiplayerSessionSubsystem::PrintString(const FString& Message) const
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, Message);
	}
}

void UMultiplayerSessionSubsystem::HandleOnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete is %s"), bWasSuccessful ? TEXT("Successfuly") : TEXT("Unsuccessfuly")));

	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			const FString MapPath = "/Game/ThirdPerson/Maps/ThirdPersonMap";
			World->ServerTravel(MapPath + "?listen");
		}
	}
}
