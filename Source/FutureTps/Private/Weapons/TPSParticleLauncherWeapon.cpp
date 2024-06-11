// FutureTPS Game All Rights Reserved

DEFINE_LOG_CATEGORY_STATIC(MyATPSParticleLauncherWeaponLog, All, All)

#include "Weapons/TPSParticleLauncherWeapon.h"

ATPSParticleLauncherWeapon::ATPSParticleLauncherWeapon()
{
	DefaultAmmo.Bullets = 1;
	DefaultAmmo.Magazines = 4;
}

void ATPSParticleLauncherWeapon::Fire() { MakeShot(); }
void ATPSParticleLauncherWeapon::SemiFire() { Super::SemiFire(); }

void ATPSParticleLauncherWeapon::StopFire()
{
	BIsUnderFire = false;
	UE_LOG(MyATPSParticleLauncherWeaponLog, Error, TEXT("StopFire"));
}

void ATPSParticleLauncherWeapon::AutoFire() {}

void ATPSParticleLauncherWeapon::MakeShot()
{
	BIsUnderFire = true;
	if (!GetWorld() || IsBulletEmpty()) { return; }

	UE_LOG(MyATPSParticleLauncherWeaponLog, Error, TEXT("Fire"));

	FVector StartLocation;
	FVector EndLocation;
	if (!GetTraceData(StartLocation, EndLocation)) { return; }

	// 射线检测碰撞点
	MakeTraceHit(LineTraceHitResult, StartLocation, EndLocation);

	// 射弹终点
	// 如果击中某个东西,那么终点就是击中点,否则就是射线的终点
	const FVector EndPoint = (LineTraceHitResult.bBlockingHit) ? LineTraceHitResult.ImpactPoint : EndLocation;

	// 射弹方向
	// 等于结束点(击中点或者射线终点)减去我们枪口的位置这条线的向量方向
	const FVector Direction = (EndPoint - GetMuzzleWorldTransform().GetLocation()).GetSafeNormal();

	// 生成射弹的Transform设置
	const FTransform SpawnTransform{FRotator::ZeroRotator, GetMuzzleWorldTransform().GetLocation()};


	if (ATPSProjectile *Projectile = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SpawnTransform))
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());

		// 绘制枪口射线 
		DrawDebugLine(GetWorld(), GetMuzzleWorldTransform().GetLocation(), EndPoint, FColor::Red,
		              false, 2,
		              0, 3);

		// 推迟actor生成,这样我们Projectile的beginPlay函数在FinishSpawning后才会调用
		// 在此之前我们可以配置东西 ,这里我们需要提前设置射弹的射击方向  
		Projectile->FinishSpawning(SpawnTransform);
	}
	DecreaseBullet();
}
