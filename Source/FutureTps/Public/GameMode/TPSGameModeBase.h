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

	FOnMatchStateChangeSignature OnMatchStateChanged;

	/// 根据控制器生成对应的AICharacter类,而不是基类(父类函数默认返回基类)
	/// 如果控制器与其相关联的AICharacter类不存在,那么生成的AICharacter将会是基类
	/// @param InController 角色的控制器
	/// @return 一个UClass的APawn
	virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;

	/// 记录玩家死亡信息到对应的PlayerState
	/// @param Killer 
	/// @param Victim 
	void KillPlayer(AController *Killer, AController *Victim);

	/// 获取当前回合数
	/// @return 
	int32 GetCurrentRound() const { return CurrentRound; }

	/// 当前回合倒计时
	/// @return 
	int32 GetRoundSecondsRemaining() const { return CurrentRoundCountDown; }

	/// 获得当前最大回合数
	/// @return 
	int32 GetMaxRound() const { return MaxRound; }

	/// 重生玩家(AI)(一般用于玩家死亡之后)
	/// @param Controller 玩家(或AI)的控制器
	void RespawnPlayer(AController *Controller);

	/// 游戏是否暂停
	/// @param PC 
	/// @param CanUnpauseDelegate 
	/// @return 
	virtual bool SetPause(APlayerController *PC, FCanUnpause CanUnpauseDelegate) override;

	/// 清除暂停,恢复游戏
	/// @return 
	virtual bool ClearPause() override;

	ETPSMatchState GetCurrentMatchState() const { return CurrentMatchState; }

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
	// 当前游戏状态
	ETPSMatchState CurrentMatchState = ETPSMatchState::WaitingToStart;

	// 当前游戏回合
	int32 CurrentRound = 0;

	int32 MaxRound = 1;

	// 当前回合倒计时
	int32 CurrentRoundCountDown = 0;

	// 回合计时器(每秒)
	FTimerHandle GameRoundTimerHandle;

	bool BIsGameOver = false;

	/// 计时器调用的函数
	/// 计算/更新游戏回合的时间
	void GameRoundTimerUpdate();

	/// 生成AI
	void SpawnBots();

	/// 开始回合
	void StartRound();

	/// 重置(重新生成)世界中的所有玩家,一般用于回合开始前
	void ResetAllPlayer();

	/// 重置(重新生成)单个玩家,一般用于回合开始前或者玩家死亡后
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

	/// 开始执行重生玩家逻辑
	/// @param Controller 玩家的控制器 
	void StartRespawn(AController *Controller);

	/// 打印世界中所有控制器对应的PlayerState
	void LogPlayerStates() const;

	/// 游戏结束
	void GameOver();

	/// 设置游戏状态
	/// @param NewState 游戏状态枚举 
	void SetMatchState(ETPSMatchState NewState);
};
