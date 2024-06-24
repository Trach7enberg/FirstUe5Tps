// FutureTPS Game All Rights Reserved

#pragma once

/**
 * 存储着游戏中的所有自定义数据结构
 */
#include "CoreType.generated.h"

// 武器逻辑组件的委托
DECLARE_MULTICAST_DELEGATE(FOnSwitchWhenChangeMagz);

class ATPSBaseWeapon;

// 武器结构
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	FString WeaponName{};

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	FString WeaponType{};

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TSubclassOf<ATPSBaseWeapon> WeaponClasses = {};

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	UAnimMontage *ReloadAnimMontage;
};

/// 武器弹药结构
USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	// 弹匣中的子弹数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	int32 Bullets = 0;

	// 弹匣数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon, meta=(ClampMin=0, ClampMax=100))
	int32 Magazines = 0;

	// 是否无限弹匣
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	bool InfiniteMag = false;
};

/// 武器UI数据,由于图片是纹理类型,所以们使用UTexture2D
USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_BODY()
	// 武器准星
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	UTexture2D *CrossIcon;

	// 显示当前武器的小图标
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	UTexture2D *CurrentWeaponIcon;
};

/// 生命组件的委托
DECLARE_MULTICAST_DELEGATE(FOnDeath);
/// 多播委托,第一个参数Health传播当前的生命值,第二个参数BIsDecreaseHealth 表示当前血量是增加还是下降
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float Health, bool BIsDecreaseHealth);
DECLARE_MULTICAST_DELEGATE(FNeedHeal);

/// VFS 特效数据结构
class UNiagaraSystem;

/// 贴花数据结构
USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_BODY()

	// C++中的材质基类是UMaterialInterface
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	UMaterialInterface *MaterialInterface;

	// 贴花尺寸
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	FVector Size = FVector(10.0f);

	// 贴花生命周期
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	float LifeTime = 5.0f;

	// 贴花逐渐消散的时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	float FadeOutTime = 0.7f;

};

/// 子弹冲击物体时的特效
USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_BODY()

	// Niagara系统
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	UNiagaraSystem *NiagaraSystemEffect;

	// 贴花结构
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	FDecalData DecalData;
};

/// 游戏模式的数据结构,存储着游戏模式的配置信息
USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_BODY()

	// 游戏玩家的数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game, meta=(ClampMin=1, ClampMax=128))
	int32 PlayersNum = 2;


	// 总回合数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game, meta=(ClampMin=1, ClampMax=16))
	int32 RoundsCount = 2;


	// 每回合时间,单位为秒
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game, meta=(ClampMin=3, ClampMax=300))
	float RoundTime = 3;

	// 重生时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game, meta=(ClampMin=2, ClampMax=60))
	float RespawnTime = 1;

	// 默认队伍颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game)
	FLinearColor DefaultTeamColor = FLinearColor::Red;

	// 队伍颜色的数组,默认有3种颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game)
	TArray<FLinearColor> TeamColors{
		FLinearColor(0.847458f, 0.231807f, 0.010489f, 1.0f),
		FLinearColor::Green,
		FLinearColor::Blue,
	};

	// 队伍ID的数组,默认只有2个队伍
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game)
	TArray<int32> TeamIDs{0, 1,};

};

/// 游戏模式的状态枚举,枚举的每个值都将存储为uint8类型
UENUM(BlueprintType)
enum class ETPSMatchState:uint8
{
	WaitingToStart = 0, // 等待游戏开始
	InProgress, // 游戏进行中
	Pause, // 游戏暂停
	GameOver, // 游戏结束
};

/// 游戏模式状态改变的委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangeSignature, ETPSMatchState);


/// 主菜单中选择关卡项的数据
USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	// 实际的关卡名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game)
	FName LevelName = NAME_None;

	// 关卡显示的名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game)
	FName LevelDisplayName = NAME_None;

	// 关卡选择的缩略图
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Game)
	UTexture2D *LevelThumbnail = nullptr;

};

/// 选择关卡时的委托,传递选择的关卡数据到需要的地方(比如主菜单)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData &);
