// FutureTPS Game All Rights Reserved


#include "PlayerStates/TPSBasePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(MyTPSBasePlayerStateLog, All, All);

void ATPSBasePlayerState::LogInfo() const
{
	UE_LOG(MyTPSBasePlayerStateLog, Error, TEXT("PlayerName:%s,TeamID:%i,Kills:%i,Deaths:%i"), *this->GetName(),
	       TeamID, KillSum, DeathSum);
}

FString ATPSBasePlayerState::GetPlayerScore() const
{
	return FString::Printf(
		TEXT("%-9s:\tTeamID:%3i,Kills:%3i,Deaths:%3i"), *this->GetPlayerName(), TeamID, KillSum, DeathSum);
}
