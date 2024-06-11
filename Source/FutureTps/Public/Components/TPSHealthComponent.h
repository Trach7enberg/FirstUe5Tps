// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreTypes/CoreType.h"
#include "TPSHealthComponent.generated.h"


UCLASS()
class FUTURETPS_API UTPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPSHealthComponent();

	// 死亡时的委托
	FOnDeath OnDeath;

	// 当血量改变时的委托
	FOnHealthChanged OnHealthChanged;

	//自动回血委托
	FNeedHeal NeedHeal;

	// 自动回血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Heal)
	bool BisAutoHealth = false;

	float GetHealth() const { return Health; }

	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	// 获取声明百分比
	UFUNCTION(BlueprintCallable, Category=Heal)
	float GetHealthPercent() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category=Heal)
	float IsFullHealth() const { return !IsDead() && FMath::IsNearlyEqual(Health, MaxHealth); }

	UFUNCTION(BlueprintCallable, Category=Heal)
	void Heal(float HealPercent) { SetHealth((HealPercent * MaxHealth) + Health); }

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health, meta=(ClampMin=0.0f, ClampMax=100.0f))
	float MaxHealth = 100.0f;

	// 回单位血量的间隔时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Heal,
		meta=(EditCondition="BisAutoHealth", ClampMin=0.000001f, ClampMax=10.0f))
	float HealUpdateTime = 0.01f;

	// 调用回血的首次延迟时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Heal,
		meta=(EditCondition="BisAutoHealth", ClampMin=0.0f, ClampMax=10.0f))
	float HealDelay = 2.0f;

	// 回血(单位)值 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Heal,
		meta=(EditCondition="BisAutoHealth", ClampMin=0.00001f, ClampMax=100.0f))
	float HealModifier = 10.0f;

private:
	float Health = 0.0f;

	// 定时器
	FTimerHandle HealTimerHandle;

	// actor接受伤害时候会 回调该函数
	UFUNCTION()
	void OnTakeAnyDamage
	(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
	 class AController *InstigatedBy, AActor *DamageCauser);

	// 回血函数
	void SetHealth(float value);

	// 回血定时器调用的 回血回调函数
	void AutoHeal();

	// 处理是否自动回血的委托回调函数
	void HealHandle();

};
