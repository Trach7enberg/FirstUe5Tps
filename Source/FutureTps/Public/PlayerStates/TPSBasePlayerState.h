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
	void AddKill() { ++KillSum; }
	void AddDeath() { ++DeathSum; }

	int32 GetKillSum() const { return KillSum; }
	void SetKillSum(const int32 KillNum) { this->KillSum = KillNum; }

	int32 GetDeathSum() const { return DeathSum; }
	void SetDeathSum(const int32 DeathNum) { this->DeathSum = DeathNum; }

	void SetTeamID(const int32 ID) { this->TeamID = ID; }
	int32 GetTeamID() const { return TeamID; }

	void SetTeamColor(const FLinearColor &Color) { this->TeamColor = Color;}
	FLinearColor GetTeamColor() const { return TeamColor; }


	void LogInfo() const;

	/// 返回玩家的得分信息
	/// @return 
	FString GetPlayerScore() const;

private:
	int32 TeamID;
	
	FLinearColor TeamColor;

	int32 KillSum = 0;
	int32 DeathSum = 0;
	
	//TODO 团队友军击杀
};
