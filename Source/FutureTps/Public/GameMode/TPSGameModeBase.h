// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CoreTypes/CoreType.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameModeBase.generated.h"

/**
 * 游戏模式类
 */
UCLASS()
class FUTURETPS_API ATPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPSGameModeBase();

	/// 游戏开始时,会调用这个函数,他在所有actor的BeginPlay之前执行
	virtual void StartPlay() override;

	/// 根据控制器生成对应的AICharacter类,而不是基类(父类函数默认返回基类)
	/// 如果控制器与其相关联的AICharacter类不存在,那么生成的AICharacter将会是基类
	/// @param InController 角色的控制器
	/// @return 一个UClass的APawn
	virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;

protected:
	/// 通过AI的控制器生成AI和销毁AI
	UPROPERTY(EditDefaultsOnly, Category=Game)
	TSubclassOf<AAIController> AIControllerClass;

	// 与AI控制器相关联的Pawn
	UPROPERTY(EditDefaultsOnly, Category=Game)
	TSubclassOf<APawn> AIPawnClass;

	/// 游戏模式的数据类
	UPROPERTY(EditDefaultsOnly, Category=Game)
	FGameData GameData;

private:
	// 当前游戏回合
	int32 CurrentRound = 1;

	// 当前回合倒计时
	int32 RoundCountDown = 0;

	// 回合计时器(每秒)
	FTimerHandle GameRoundTimerHandle;

	/// 计时器调用的函数
	void GameRoundTimerUpdate();

	/// 生成AI
	void SpawnBots();

	/// 开始回合
	void StartRound();

	/// 重置(重新生成)世界中的所有玩家,一般用于回合开始前
	void ResetAllPlayer();

	/// 重置(重新生成)单个玩家,一般用于回合开始前
	void ResetPlayer(AController *Controller);


	/// 初始化所有玩家的PlayerState(队伍ID、颜色)
	void InitAllPlayerState();

	/// 通过player的团队ID分配队伍颜色
	/// @param TeamID 
	/// @return 
	FLinearColor GetColorByTeamID(const int32 TeamID);

	/// 设置玩家的颜色
	/// @param Controller 
	void SetPlayerColor(const AController *Controller);

};
