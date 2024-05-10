// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuuccessFull);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult> & sessionResults, bool bWasSuccessfull);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessfull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessfull);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
	UMultiplayerSessionSubsystem();
	void CreateSession(int32 numPublicConnection, FString matchType);
	void FindSessions(int32 maxSearchResult);
	void JoinSession(const FOnlineSessionSearchResult& sessionResult);
	void DestroySession();
	void StartSession();

	FMultiplayerOnCreateSessionComplete multiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete multiplayerOnFindSessionComplete;
	FMultiplayerOnJoinSessionComplete multiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete multiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete multiplayerOnStartSessionComplete;

	protected:
	void OnCreateSessionComplete(FName sessionName,  bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	void OnDestroySessionComplete(FName sessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName sessionName, bool bWasSuccessful);

	private:
	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSettings> lastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> lastSessionSearch;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	FDelegateHandle createSessionCompleteDelegate;
	FDelegateHandle findSessionCompleteDelegate;
	FDelegateHandle joinSessionCompleteDelegate;
	FDelegateHandle destroySessionCompleteDelegate;
	FDelegateHandle startSessionCompleteDelegate;

	bool bCreateSessionOnDestroy{false};
	int32 lastNumPublicConnections;
	FString lastMatchType;
};
