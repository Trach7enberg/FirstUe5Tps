// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TPSBaseWeapon.h"
#include "TPSRifleWeapon.generated.h"

class UWeaponFXComponent;
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

	// 特效组件,利用它来播放特效
	UPROPERTY(VisibleAnywhere,Category=VFX)
	UWeaponFXComponent * WeaponFXComponent;
	
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
};
