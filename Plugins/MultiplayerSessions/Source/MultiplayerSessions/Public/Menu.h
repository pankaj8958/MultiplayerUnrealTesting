// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 numberOfPublicConnections = 4, FString typeOfMatch = FString(TEXT("FreeForAll")));

	protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	//
	//Callbacks for custom delegates on the multiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSessionComplete(bool bSuccessful);
	void OnFindSessionComplete(const TArray<FOnlineSessionSearchResult> & sessionResults, bool bWasSuccessfull);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type result);
	UFUNCTION()
	void OnDestroySessionComplete(bool bSuccessful);
	UFUNCTION()
	void OnStartSessionComplete(bool bSuccessful);

	private:
	UPROPERTY(meta= (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta= (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClick();
	UFUNCTION()
	void JoinButtonClick();
	
	void MenuTearDown();

	class UMultiplayerSessionSubsystem* multiplayerSessionSubsystem;

	int32 numPublicConnections{4};
	FString matchType{TEXT("FreeForAll")};
};
