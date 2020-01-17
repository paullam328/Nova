// Copywrite Shapeshifter 2019

#include "SyncNetworkServerClient.h"

USyncNetworkServerClient::USyncNetworkServerClient()
{
	FNetworkVersion::GetLocalNetworkVersionOverride.BindUObject(this, &USyncNetworkServerClient::GetServerClientSyncVersion);
}


uint32 USyncNetworkServerClient::GetServerClientSyncVersion()
{
	return 100000;
}