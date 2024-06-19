// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayerHUDWidget.generated.h"

struct FAmmoData;
struct FWeaponUIData;

/**
 * 玩家UI类,玩家的UIWidget蓝图继承自这个类 
 */
UCLASS()
class FUTURETPS_API UTPSPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/// 本类初始化函数
	virtual bool Initialize() override;
	///  获取玩家生命值
	/// @return 生命值
	UFUNCTION(BlueprintCallable, Category=UI)
	float GetHealthPercent() const;

	/// 获取玩家武器UI数据(图标和当前武器指示)
	/// @param WeaponUIData 
	/// @return 武器UI数据结构
	UFUNCTION(BlueprintCallable, Category=UI)
	bool GetCurrentWeaponUIData(FWeaponUIData &WeaponUIData) const;

	/// 获取玩家武器子弹数据
	/// @param AmmoData 
	/// @return 弹药数据结构
	UFUNCTION(BlueprintCallable, Category=UI)
	bool GetCurrentWeaponAmmo(FAmmoData &AmmoData) const;


	/// 获取已经处理好的弹药信息文本
	/// @return 返回显示弹药的文本
	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetCurrentAmmoText() const;


	/// 设置玩家正常游玩视角时UI的visibility
	/// @return 人物死亡则为Hidden,反之为正常显示
	UFUNCTION(BlueprintCallable, Category=UI)
	ESlateVisibility SettingPlayerVisibility() const;

	/// 设置玩家进入观察模式时UI的visibility
	/// @return 
	UFUNCTION(BlueprintCallable, Category=UI)
	ESlateVisibility SettingSpectatorVisibility() const;

	/// 是否在换弹
	/// @return 
	UFUNCTION()
	ESlateVisibility IsReloading() const;


	/// 在widget蓝图类里实现方法体,用于通知当前(扣血了),即受伤 显示受伤动画
	UFUNCTION(BlueprintImplementableEvent, Category=UI)
	void OnHealthChanged(float Health, bool BIsDecreaseHealth);


	
private:
	/// 
	/// @return 
	UFUNCTION(BlueprintCallable, Category=UI)
	bool IsPlayerAlive() const;

	/// 
	/// @return 
	UFUNCTION(BlueprintCallable, Category=UI)
	bool IsPlayerSpectating() const;
	
	
	/// 当生命值变化时生命组件回调的函数
	/// @param health 
	/// @param BIsDecreaseHealth 
	UFUNCTION(BlueprintCallable, Category=UI)
	void HealthChanged(float health, bool BIsDecreaseHealth);

};
