// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayerStatisticsWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 统计角色信息(击杀、死亡等)的数据行界面
 */
UCLASS()
class FUTURETPS_API UTPSPlayerStatisticsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTeamNameText(const FString & TeamName) const;
	void SetPlayerNameText(const FString & PlayerName) const;
	void SetKillsText(const FString & Kills) const;
	void SetDeathsText(const FString & Deaths) const;

	void SetPlayerIndicatorImageVisibility(bool Visible=false) const;

protected:
	/// (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UTextBlock *TeamNameText;

	/// (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UTextBlock *PlayerNameText;

	/// (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UTextBlock *KillsText;

	/// (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UTextBlock *DeathsText;

	/// (自动绑定到蓝图中的UI组件)
	UPROPERTY(meta=(BindWidget))
	UImage * PlayerIndicatorImage;
};
