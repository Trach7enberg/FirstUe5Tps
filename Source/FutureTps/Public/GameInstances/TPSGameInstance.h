// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

class UButton;
/**
 * 游戏实例类, 用于保存游戏全局数据
 */
UCLASS()
class FUTURETPS_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FName GetDefaultMapName() const { return DefaultMapName; }

	FName GetMainMenuMapName() const { return MainMenuMapNam; }

private:
	// 默认地图名称
	UPROPERTY(EditDefaultsOnly, Category=Game)
	FName DefaultMapName = NAME_None;

	// 主菜单地图名称
	UPROPERTY(EditDefaultsOnly, Category=Game)
	FName MainMenuMapNam = NAME_None;
};
