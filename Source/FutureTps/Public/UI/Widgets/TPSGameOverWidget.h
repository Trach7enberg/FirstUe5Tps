// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSGameOverWidget.generated.h"

class UButton;
enum class ETPSMatchState : uint8;
class UVerticalBox;
/**
 * 游戏结束界面
 */
UCLASS()
class FUTURETPS_API UTPSGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	/// 玩家统计信息的区域 (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UVerticalBox *StatisticsArea;

	/// 玩家信息行的Widget UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> PlayerStatisticsRowWidgetClass;

	/// 重新开始关卡的按钮
	UPROPERTY(meta=(BindWidget))
	UButton *ResetLevelButton;


private:
	void OnMatchStateChanged(ETPSMatchState MatchState);

	/// 更新玩家统计信息(击杀、死亡等)
	void UpdatePlayerStat();

	/// 重新开始关卡按钮的回调函数
	UFUNCTION()
	void OnResetLevel();
};
