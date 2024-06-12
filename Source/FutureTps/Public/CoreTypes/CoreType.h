// FutureTPS Game All Rights Reserved

#pragma once

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

/// VFS
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

/// VFS
