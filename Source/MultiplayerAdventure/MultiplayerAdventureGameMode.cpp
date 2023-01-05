// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerAdventureGameMode.h"
#include "MultiplayerAdventureCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerAdventureGameMode::AMultiplayerAdventureGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMultiplayerAdventureGameMode::HostLANGame(const FString& MapPath)
{
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(MapPath + "?listen");
	}
}

void AMultiplayerAdventureGameMode::JoinLANGame(const FString& Address)
{
	if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(Address, TRAVEL_Absolute);
	}
}
