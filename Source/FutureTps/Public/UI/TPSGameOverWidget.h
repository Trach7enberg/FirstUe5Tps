// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSGameOverWidget.generated.h"

enum class ETPSMatchState : uint8;
class UVerticalBox;
/**
 * 游戏结束界面
 */
UCLASS()
class FUTURETPS_API UTPSGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	/// 玩家统计信息的区域 (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UVerticalBox *StatisticsArea;

	/// 玩家信息行的Widget UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> PlayerStatisticsRowWidgetClass;

private:
	void OnMatchStateChanged(ETPSMatchState MatchState);

	/// 更新玩家统计信息(击杀、死亡等)
	void UpdatePlayerStat();
};
