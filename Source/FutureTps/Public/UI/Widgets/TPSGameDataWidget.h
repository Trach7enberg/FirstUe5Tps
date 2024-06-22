// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSGameDataWidget.generated.h"

class ATPSGameModeBase;
class ATPSBasePlayerState;

/**
 * 显示关于游戏数据的UI,例如倒计时、得分等
 */
UCLASS()
class FUTURETPS_API UTPSGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetRoundText() const;

	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetKillText() const;

	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetCountDownText() const;

private:
	/// 获取游戏模式
	/// @return 
	ATPSGameModeBase * GetAtpsGameMode() const;

	/// 获取玩家状态
	ATPSBasePlayerState *GetPlayerState() const;
};
