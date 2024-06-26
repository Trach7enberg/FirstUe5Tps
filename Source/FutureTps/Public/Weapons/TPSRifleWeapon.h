// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TPSBaseWeapon.h"
#include "TPSRifleWeapon.generated.h"

class UWeaponFXComponent;
class UNiagaraSystem;
class UAudioComponent;
/**
 *  步枪类
 */
UCLASS()
class FUTURETPS_API ATPSRifleWeapon : public ATPSBaseWeapon
{
	GENERATED_BODY()

public:
	explicit ATPSRifleWeapon();

	virtual void Fire() override;
	virtual void SemiFire() override;
	virtual void StopFire() override;
	virtual void AutoFire() override;

protected:
	virtual void BeginPlay() override;

	// 用来播放物体冲击表面的特效(贴花之类的)
	UPROPERTY(VisibleAnywhere, Category=VFX)
	UWeaponFXComponent *WeaponFXComponent;

	// 子弹轨迹特效类资源
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	UNiagaraSystem *BulletTraceNiagaraSystem;
	
	// 这个是Niagara系统中创建的变量的名字
	// 我们将通过这个名字修改特效系统里的变量值
	UPROPERTY(VisibleAnywhere, Category=VFX)
	FName BeamEndName = "TraceTarget";
	
	// 自动开火速率
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin = 0.01f, ClampMax=0.1f))
	float FireRate = 0.05f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WeaponDamage = 10.f;

	// 子弹散射角度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin = 0.5f, ClampMax=3.0f))
	float BulletSpread = 1.5f;

	virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd, float HalfConeAng = 0) const override;

	virtual void MakeShot() override;

	void MakeDamage(const FHitResult &HitResult);

private:
	FTimerHandle AutoFireTimer;
	// 光线击中时返回的击中数据
	FHitResult LineTraceHitResult;

	// 保存枪口火花特效的实体
	UPROPERTY()
	UNiagaraComponent *RifleMuzzleFX = nullptr;

	// TODO 有bug待修复,SpawnSoundAttach返回的组件不会跟随武器移动
	UPROPERTY()
	UAudioComponent *FireSoundAudioComponent = nullptr;

	/// 初始化(生成)枪口特效 暂存到RifleMuzzleFX
	void InitRifleMuzzleFX();

	/// 初始化(获得)开火声音 暂存到FireSoundAudioComponent
	void InitRifleFireSound();

	/// 是否播放开火声音
	/// @param Active 
	void SetSoundActive(bool Active) const;

	/// 设置枪口特效可见度
	void SetFXActive(bool Visible) const;

	/// 生成子弹轨迹特效
	/// @param Start 
	/// @param End 
	void SpawnBulletTraceFX(const FVector &Start, const FVector &End) const;
};
