// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TPSBaseWeapon.h"
#include "Projectiles/TPSProjectile.h"
#include "TPSParticleLauncherWeapon.generated.h"

/**
 * 武器:粒子发射器类
 */
UCLASS()
class FUTURETPS_API ATPSParticleLauncherWeapon : public ATPSBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire() override;
	virtual void SemiFire() override;
	virtual void StopFire() override;
	virtual void AutoFire() override;

protected:
	// 射弹实体
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<ATPSProjectile> ProjectileClass = nullptr;

	virtual void MakeShot() override;

public:
	ATPSParticleLauncherWeapon();

private:
	// 光线击中时返回的击中数据
	FHitResult LineTraceHitResult;

	
};
