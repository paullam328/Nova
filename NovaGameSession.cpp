// Copywrite Shapeshifter 2019

#include "NovaGameSession.h"
#include "Runtime/Core/Public/Internationalization/Text.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "CharacterSelectionGameMode.h"

// Constructor 
ANovaGameSession::ANovaGameSession()
{

}

void ANovaGameSession::RegisterServer()
{
	Super::RegisterServer();
	UE_LOG(LogTemp, Warning, TEXT("****************************Session created**************************"));


	/*TSharedPtr<const FUniqueNetId> UserId;

	auto OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		auto IdentityInterface = OnlineSubsystem->GetIdentityInterface();
		if (IdentityInterface.IsValid())
		{
			UserId = IdentityInterface->GetUniquePlayerId(0);
		}
	}*/



	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) 
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
				FOnlineSessionSettings* HostSettings = new FOnlineSessionSettings();

				HostSettings->bUsesPresence = false;
				HostSettings->bIsLANMatch = false;
				HostSettings->bIsDedicated = true;
				HostSettings->bShouldAdvertise = true;
				HostSettings->bAllowJoinInProgress = false;
				HostSettings->NumPublicConnections = 6;
				Sessions->CreateSession(0, GameSessionName, *HostSettings);
				UE_LOG(LogTemp, Warning, TEXT("****************************Session created**************************"));
		}
	}


}

// Kick Function
//bool ANovaGameSession::KickPlayer(APlayerController* KickedPlayer, const FText& KickReason)
//{
//
//
//	//KickReason = FText("Quiting the Session");
//
//	Super::KickPlayer(KickedPlayer, KickReason);
//
//
//	return true;