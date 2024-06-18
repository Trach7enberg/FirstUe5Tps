// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSBasePlayerState.generated.h"

/**
 *  玩家状态类
 */
UCLASS()
class FUTURETPS_API ATPSBasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetTeamID(const int32 ID) { this->TeamID = ID; }
	void SetTeamColor(const FLinearColor &Color) { this->TeamColor = Color; }

	int32 GetTeamID() const { return TeamID; }
	FLinearColor GetTeamColor() const { return TeamColor; }

private:
	int32 TeamID;

	FLinearColor TeamColor;


};
