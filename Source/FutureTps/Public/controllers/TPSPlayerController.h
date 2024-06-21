
// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

enum class ETPSMatchState : uint8;
class UTPSRespawnComponent;
/**
 * 玩家的控制器
 */
UCLASS()
class FUTURETPS_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();

public:
	// 重生组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UTPSRespawnComponent* RespawnComponent;
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	/// 暂停游戏
	void PauseGame();

	/// 当游戏状态改变时调用该函数,主要作用是根据游戏状态改变鼠标是否显示
	/// @param MatchState 
	void OnMatchStateChanged(ETPSMatchState MatchState);
};
