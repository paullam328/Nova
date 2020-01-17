// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NetworkVersion.h"
#include "SyncNetworkServerClient.generated.h"

/**
 * 
 */
UCLASS()
class NOVAFINAL_API USyncNetworkServerClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	USyncNetworkServerClient();

private:
	/*
	* Client Cannot connect to Server if this number not fit
	*/

	uint32 GetServerClientSyncVersion();
	
};
