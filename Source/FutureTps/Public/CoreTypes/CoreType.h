// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreType.generated.h"

// 武器逻辑组件的委托
DECLARE_MULTICAST_DELEGATE(FOnSwitchWhenChangeMagz);

class ATPSBaseWeapon;
// 武器结构
USTRUCT(Blueprintable)
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
USTRUCT(Blueprintable)
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
USTRUCT(Blueprintable)
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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
DECLARE_MULTICAST_DELEGATE(FNeedHeal);