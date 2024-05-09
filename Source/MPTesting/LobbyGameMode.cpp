// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController *newPlayer)
{
    Super::PostLogin(newPlayer);
    if(GameState)
    {
        int32 numOfPlayers = GetNumPlayers();
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                1,
                60.f,
                FColor::Yellow,
                FString::Printf(TEXT("Player total: %d"), numOfPlayers)
            );
        }
        APlayerState* playerState = newPlayer->GetPlayerState<APlayerState>();
        if(playerState)
        {
            FString playerName = playerState->GetPlayerName();
            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    60.f,
                    FColor::Cyan,
                    FString::Printf(TEXT("Player Joined : %s"), *playerName)
                );
            }
        }
    }
}

void ALobbyGameMode::Logout(AController *exiting)
{
    Super::Logout(exiting);
    APlayerState* playerState = exiting->GetPlayerState<APlayerState>();
    if(playerState)
    {
        int32 numOfPlayers = GetNumPlayers();
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                1,
                60.f,
                FColor::Yellow,
                FString::Printf(TEXT("Player total: %d"), numOfPlayers - 1)
            );
        }
        FString playerName = playerState->GetPlayerName();
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                60.f,
                FColor::Cyan,
                FString::Printf(TEXT("Player exited : %s"), *playerName)
            );
        }
        
    }
}
