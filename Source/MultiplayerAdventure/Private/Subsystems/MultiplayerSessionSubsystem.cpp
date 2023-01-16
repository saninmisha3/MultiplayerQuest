// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

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
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::HandleOnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::HandleOnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::HandleOnJoinSessionComplete);
			PrintString("Session Interface is valid!");
		}
	}
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMultiplayerSessionSubsystem::CreateServer(const FString& ServerName, const FString& InTravelMapPath)
{
	PrintString("Creating Server ...");

	if (InTravelMapPath.IsEmpty())
	{
		PrintString("Travel Map is empty!", true);
		OnServerCreateCompleted.Broadcast(false);
		return;
	}
	TravelMapPath = InTravelMapPath;
	
	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!", true);
		OnServerCreateCompleted.Broadcast(false);
		return;
	}

	// If there exists a session with this name, it need to be destroyed.
	if (SessionInterface->GetNamedSession(OurSessionName))
	{
		PrintString("Session already exists, destroying it...", true);
		RecreateServerName = ServerName;
		bRecreateServer = true;
		SessionInterface->DestroySession(OurSessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true; // Is match is public
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const bool bIsLAN = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	SessionSettings.bIsLANMatch = bIsLAN; 
	
	SessionInterface->CreateSession(0, OurSessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::JoinServer(const FString& ServerName)
{
	PrintString("Joining Server ...");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!", true);
		OnJoinServerCompleted.Broadcast(false);
		return;
	}

	ServerNameToFind = ServerName;

	SessionSearch = MakeShareable(new FOnlineSessionSearch);
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionSubsystem::HandleOnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete is %s"), bWasSuccessful ? TEXT("Successfuly") : TEXT("Unsuccessfuly")), !bWasSuccessful);

	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(TravelMapPath + "?listen");
		}
	}

	OnServerCreateCompleted.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionSubsystem::HandleOnDestroySessionComplete(const FName SessionName, const bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnDestroySessionComplete is %s"), bWasSuccessful ? TEXT("Successfuly") : TEXT("Unsuccessfuly")), !bWasSuccessful);

	// Recreate a server if this delegate was called when we were trying to create an existing session
	if (bRecreateServer)
	{
		CreateServer(RecreateServerName, TravelMapPath);
		RecreateServerName = FString();
		bRecreateServer = false;
	}
}

void UMultiplayerSessionSubsystem::HandleOnFindSessionsComplete(const bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnFindSessionsComplete is %s"), bWasSuccessful ? TEXT("Successfuly") : TEXT("Unsuccessfuly")), !bWasSuccessful);

	if (!bWasSuccessful || ServerNameToFind.IsEmpty())
	{
		OnJoinServerCompleted.Broadcast(false);
		return;
	}

	const TArray<FOnlineSessionSearchResult>& SearchResults = SessionSearch->SearchResults;
	if (!SearchResults.IsEmpty())
	{
		FOnlineSessionSearchResult* CorrectResult = nullptr;
		PrintString(FString::Printf(TEXT("%d sessions found."), SearchResults.Num()));

		for (FOnlineSessionSearchResult Result : SearchResults)
		{
			if (Result.IsValid())
			{
				FString ServerName;
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				if (ServerName.Equals(ServerNameToFind))
				{
					CorrectResult = &Result;
					PrintString(FString::Printf(TEXT("Found server with name: %s"), *ServerName));
					break;
				}
			}
		}

		if (CorrectResult && CorrectResult->IsValid())
		{
			SessionInterface->JoinSession(0, OurSessionName, *CorrectResult);
			return;
		}
		else
		{
			PrintString(FString::Printf(TEXT("Couldn't find server: %s"), *ServerNameToFind), true);
			ServerNameToFind = FString();
		}
	}
	else
	{
		PrintString("Zero sessions found.", true);
	}
	
	OnJoinServerCompleted.Broadcast(false);
}

void UMultiplayerSessionSubsystem::HandleOnJoinSessionComplete(const FName SessionName, const EOnJoinSessionCompleteResult::Type JoinResult)
{
	const bool bWasSuccessful = JoinResult == EOnJoinSessionCompleteResult::Success;
	PrintString(FString::Printf(TEXT("OnJoinSessionComplete is %s"), bWasSuccessful ? TEXT("Successfuly") : TEXT("Unsuccessfuly")), !bWasSuccessful);

	if (bWasSuccessful)
	{
		FString Address;
		if (SessionInterface->GetResolvedConnectString(SessionName, Address))
		{
			PrintString(FString::Printf(TEXT("Address: %s"), *Address));
			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				// CAN'T CONNECT ISSUE: If you can't join to the session but everything is successful -> Close Steam Client (For NULL OSS)
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
				OnJoinServerCompleted.Broadcast(true);
				return;
			}
		}
		else
		{
			PrintString("GetResolvedConnectString returned false!", true);
		}
	}

	OnJoinServerCompleted.Broadcast(false);
}

void UMultiplayerSessionSubsystem::PrintString(const FString& Message, const bool bIsError) const
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, bIsError ? FColor::Red : FColor::Green, Message);
	}
}
