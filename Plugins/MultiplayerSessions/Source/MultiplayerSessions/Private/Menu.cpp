// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
void UMenu::MenuSetup(int32 numberOfPublicConnections, FString typeOfMatch)
{
    numPublicConnections = numberOfPublicConnections;
    matchType = typeOfMatch;
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    
    UWorld* world = GetWorld();
    if(world)
    {
        APlayerController* playerController = world->GetFirstPlayerController();
        if(playerController)
        {
            FInputModeUIOnly inputModeData;
            inputModeData.SetWidgetToFocus(TakeWidget());
            inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            playerController->SetInputMode(inputModeData);
            playerController->SetShowMouseCursor(true);
        }
    }
    UGameInstance* gameInstance = GetGameInstance();
    if(gameInstance)
    {
        multiplayerSessionSubsystem = gameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
    }
    if(multiplayerSessionSubsystem)
    {
        multiplayerSessionSubsystem->multiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSessionComplete);
        multiplayerSessionSubsystem->multiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSessionComplete);
        multiplayerSessionSubsystem->multiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
        multiplayerSessionSubsystem->multiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySessionComplete);
        multiplayerSessionSubsystem->multiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSessionComplete);
    }
}

bool UMenu::Initialize()
{
    if(!Super::Initialize())
    {
        return false;
    }
    if(HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClick);
    }
    if(JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClick);
    }
    
    return true;
}

void UMenu::NativeDestruct()
{
    MenuTearDown();
    Super::NativeDestruct();
}

void UMenu::OnCreateSessionComplete(bool bSuccessful)
{
    if(bSuccessful)
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Yellow,
                FString(TEXT("Session Create Successfully"))
            );
            
            UWorld* world = GetWorld();
            if(world)
            {
                world->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
            }
        }
    } else
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Red,
                FString(TEXT("Session Create Failed"))
            );
        }
    }
}

void UMenu::OnFindSessionComplete(const TArray<FOnlineSessionSearchResult> &sessionResults, bool bWasSuccessfull)
{
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Yellow,
            FString(TEXT("Find Session Complete1"))
        );
    }
    if(multiplayerSessionSubsystem == nullptr)
    {
        return;
    }
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Yellow,
            FString::Printf(TEXT("On FindSession  count %d"), sessionResults.Num())
        );
    }
    for (auto result :  sessionResults)
    {
        FString settingValue;
        result.Session.SessionSettings.Get(FName("MatchType"), settingValue);
        if(settingValue == matchType)
        {
            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Yellow,
                    FString(TEXT("Match type found"))
                );
            }
            multiplayerSessionSubsystem->JoinSession(result);
            return;
        } else
        {
            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Yellow,
                    FString::Printf(TEXT("On FindSessionComplete setting  %s"), *settingValue)
                );
            }
        }
        
    }
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Yellow,
            FString(TEXT("Match type Not found"))
        );
    }
}

void UMenu::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type result)
{
    IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
    if(subsystem)
    {
        IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
        if(sessionInterface.IsValid())
        {
            FString address;
            sessionInterface->GetResolvedConnectString(NAME_GameSession, address);

            APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
            if(playerController)
            {
                playerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}

void UMenu::OnDestroySessionComplete(bool bSuccessful)
{
}

void UMenu::OnStartSessionComplete(bool bSuccessful)
{
}

void UMenu::HostButtonClick()
{
    if(multiplayerSessionSubsystem)
    {
        multiplayerSessionSubsystem->CreateSession(numPublicConnections, matchType);
    }
}

void UMenu::JoinButtonClick()
{
    if(multiplayerSessionSubsystem == nullptr)
    {
        return;
    }
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Yellow,
            FString(TEXT("Join Button Clicked"))
        );
    }
    if(multiplayerSessionSubsystem)
    {
        multiplayerSessionSubsystem->FindSessions(1000);
    }
}

void UMenu::MenuTearDown()
{
    RemoveFromParent();
    UWorld* world = GetWorld();
    if(world)
    {
        APlayerController* playerController = world->GetFirstPlayerController();
        if(playerController)
        {
            FInputModeGameOnly inputMode;
            playerController->SetInputMode(inputMode);
            playerController->SetShowMouseCursor(false);
        }
    }
}
