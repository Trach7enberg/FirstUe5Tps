// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayerHUDWidget.generated.h"

class UProgressBar;
struct FAmmoData;
struct FWeaponUIData;

/**
 * 玩家主显示UI类,玩家的UIWidget蓝图继承自这个类 
 */
UCLASS()
class FUTURETPS_API UTPSPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
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


	/// 获取已经处理好的弹药(弹匣中的子弹)信息文本
	/// @return 返回显示弹药的文本
	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetCurrentBulletsText() const;

	/// 获取已经处理好的弹药(弹匣)信息文本
	/// @return
	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetCurrentMagzsText() const;


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
	UFUNCTION(BlueprintCallable, Category=UI)
	ESlateVisibility IsReloading() const;


	/// 在widget蓝图类里实现方法体,用于通知当前(扣血了),即受伤 显示受伤动画
	UFUNCTION(BlueprintImplementableEvent, Category=UI)
	void OnHealthChanged(float Health, bool BIsDecreaseHealth);

protected:
	/// 初始化函数,Initialize 成功执行初始化时候会调用这个函数
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	UProgressBar *PlayerHealthBar;

	// 生命值小于等于这个数会改变颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	float BarColorChangePercent = 0.3f;

	// 生命值满时的颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	FLinearColor FullHealthColor = FLinearColor::Black;

	// 生命值低于BarColorChangePercent时的颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	FLinearColor LowHealthColor = FLinearColor::Red;

private:
	/// 
	/// @return 
	bool IsPlayerAlive() const;

	/// 
	/// @return 
	bool IsPlayerSpectating() const;


	/// 当生命值变化时生命组件回调的函数
	/// @param Health 
	/// @param BIsDecreaseHealth 
	UFUNCTION(BlueprintCallable, Category=UI)
	void HealthChanged(float Health, bool BIsDecreaseHealth);

	/// 当玩家控制新的角色的时候(比如死亡复活之后)会触发这个回调函数
	/// @param NewPawn 
	UFUNCTION()
	void OnNewPawn(APawn *NewPawn);

	/// 更新玩家生命值UI的颜色和百分比
	void UpdatePlayerHealthBar() const;
};
