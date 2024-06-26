// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/CoreType.h"
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
	FLevelData GetStartLevel() const { return StartLevel; }

	/// 设置当前要开始游戏的关卡的数据
	/// @param Data 关卡数据
	void SetStartLevel(const FLevelData &Data) { StartLevel = Data; }

	FName GetMainMenuLevelName() const { return MainMenuMapNam; }

	/// @brief 获取所有关卡数据
	/// @return 关卡数据数组
	TArray<FLevelData> GetLevelsData() const { return LevelsData; }

	// 切换主音量
	void ToggleVolume() const;
protected:
	// 被选择开始游戏的关卡的数据
	UPROPERTY(EditDefaultsOnly, Category=Game,meta=(ToolTip="关卡的名字(LevelName)应该是独一无二的"))
	TArray<FLevelData> LevelsData;

	// 主音量
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Sound)
	USoundClass* MasterSoundClass;
	
	// 主菜单地图名称
	UPROPERTY(EditDefaultsOnly, Category=Game)
	FName MainMenuMapNam = NAME_None;

private:
	// 默认关卡
	FLevelData StartLevel{};


};
