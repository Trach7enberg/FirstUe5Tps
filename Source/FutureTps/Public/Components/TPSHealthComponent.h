// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreTypes/CoreType.h"
#include "TPSHealthComponent.generated.h"

class UCameraShakeBase;
class UPhysicalMaterial;

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


	// 获取生命百分比值
	UFUNCTION(BlueprintCallable, Category=Heal)
	float GetHealthPercent() const { return Health / MaxHealth; }

	///  判断当前actor是否满血
	/// @return 是则返回true,否则false
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	TSubclassOf<UCameraShakeBase> CameraShake;

	// 物理材质对应的伤害倍数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	TMap<UPhysicalMaterial *, float> DamagedModifiers;

private:
	float Health = 0.0f;


	// 定时器
	FTimerHandle HealTimerHandle;

	void ApplyDamage(float Damage, AController *InstigatedBy);

	// actor接受伤害时候会 回调该函数
	UFUNCTION()
	void OnTakeAnyDamage
	(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
	 class AController *InstigatedBy, AActor *DamageCauser);

	UFUNCTION()
	void OnTakePointDamage(AActor *DamagedActor, float Damage, class AController *InstigatedBy, FVector HitLocation,
	                       class UPrimitiveComponent *FHitComponent, FName BoneName, FVector ShotFromDirection,
	                       const class UDamageType *DamageType, AActor *DamageCauser);

	UFUNCTION()
	void OnTakeRadialDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, FVector Origin,
	                        const FHitResult &HitInfo, class AController *InstigatedBy, AActor *DamageCauser);

	// 回血函数
	void SetHealth(float value);

	// 回血定时器调用的 回血回调函数
	void AutoHeal();

	// 处理是否自动回血的委托回调函数
	void HealHandle();

	/// 播放抖动摄像机
	void OscillationCamera() const;

	/// 击杀玩家或者AI,记录击杀信息
	/// @param Killer 
	void KillPlayer(AController *Killer);

	/// 根据TMap里映射好的关系获取当前骨骼点的伤害倍数
	/// @param BoneName 
	/// @param DamagedActor 
	/// @return 伤害倍数
	float GetPointDamagedModifiers(const FName & BoneName,AActor *DamagedActor) const;

	void ReportDamageEvent(float Damage, const AController *InstigatedBy) const;
};
