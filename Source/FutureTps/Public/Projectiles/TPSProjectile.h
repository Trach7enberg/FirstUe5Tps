// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TPSProjectile.generated.h"


class UNiagaraComponent;
class UWeaponFXComponent;
/**
 * 射弹类实体
 */
UCLASS()
class FUTURETPS_API ATPSProjectile : public AActor
{
	GENERATED_BODY()

public:
	ATPSProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	USphereComponent *CollisionComponent;

	// 射弹的粒子轨迹特效
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=VFX)
	// UNiagaraComponent *ParticleTraceComponent;


	void SetShotDirection(const FVector &Direction);

protected:
	// 控制射弹移动的组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	UProjectileMovementComponent *ProjectileMovementComponent;

	// 特效组件,利用它来播放特效
	UPROPERTY(VisibleAnywhere, Category=VFX)
	UWeaponFXComponent *WeaponFXComponent;

	// 球型碰撞体半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	float SphereRadius = 5.f;

	// 射弹移动方向
	FVector ShotDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	float DamageValue = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	bool BIsDoFullDamage = false;

	/// 射弹击中物体时候就会调用这个函数 , (射弹在发射器类里生成)
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
	                     FVector NormalImpulse, const FHitResult &Hit);

	AController *GetPlayerController() const;


	virtual void BeginPlay() override;

private:
	float LifeSec = 3.0f;
};
