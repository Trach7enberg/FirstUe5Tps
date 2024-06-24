// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSGameHUD.generated.h"


class UTPSBaseWidget;
enum class ETPSMatchState : uint8;
class UUserWidget;
/**
 * 管理各个UI的主类
 */
UCLASS()
class FUTURETPS_API ATPSGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	/// 玩家Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> PlayHUDWidgetClass;

	// 游戏暂停时的Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> PauseHUDWidgetClass;

	// 游戏结束时的Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> GameOVerHUDWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<ETPSMatchState, UTPSBaseWidget *> MatchStateWidgets;

	UPROPERTY()
	UTPSBaseWidget *CurrentMatchStateWidget = nullptr;

	void DrawCrossHair();

	void OnMatchStateChanged(ETPSMatchState MatchState);

};
