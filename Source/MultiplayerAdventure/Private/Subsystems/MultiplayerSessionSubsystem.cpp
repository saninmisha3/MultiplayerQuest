// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/MultiplayerSessionSubsystem.h"

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
			PrintString("Session Interface is valid!");
		}
	}
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMultiplayerSessionSubsystem::PrintString(const FString& Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, Message);
	}
}
