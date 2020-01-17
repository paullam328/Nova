// Copywrite Shapeshifter 2019

#include "NovaGameViewportClient.h"


UNovaGameViewportClient::UNovaGameViewportClient()
{

}

TOptional<bool> UNovaGameViewportClient::QueryShowFocus(const EFocusCause InFocusCause) const
{
	return false;
}