// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSMenuWidget.generated.h"

class UButton;
/**
 * 主菜单UI小部件
 */
UCLASS()
class FUTURETPS_API UTPSMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	/// 开始游戏按钮
	UPROPERTY(meta=(BindWidget))
	UButton *StartGameButton;

	/// 默认打开的关卡名字
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=StartLevelName)
	FName StartLevelName = "Tmap";

private:
	UFUNCTION()
	void OnStartGame();
};
