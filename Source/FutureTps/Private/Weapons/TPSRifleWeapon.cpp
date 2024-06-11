// FutureTPS Game All Rights Reserved


#include "Weapons/TPSRifleWeapon.h"
#include "Characters/TPSBaseCharacter.h"
#include "Effects/TPSWeaponFXComponent.h"


DEFINE_LOG_CATEGORY_STATIC(MyATPSRifleWeaponLog, All, All)

class ATPSBaseCharacter;

ATPSRifleWeapon::ATPSRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("WeaponFXComponent");
}

void ATPSRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ATPSRifleWeapon::Fire() { AutoFire(); }

void ATPSRifleWeapon::SemiFire()
{
	if (GetOwner())
	{

		// 防止角色死亡后左键没有松手,还会继续开枪,需要清除定时器
		if (IsBulletEmpty() || GetOwner()->GetLifeSpan() != 0.0f)
		{
			UE_LOG(MyATPSRifleWeaponLog, Error, TEXT("Empty>?"));
			StopFire();
			return;
		}
	}
	MakeShot();
}

void ATPSRifleWeapon::StopFire()
{
	if (!GetWorld()) { return; }
	GetWorld()->GetTimerManager().ClearTimer(AutoFireTimer);
	UE_LOG(MyATPSRifleWeaponLog, Error, TEXT("StopFire"));

	BIsUnderFire = false;
}

void ATPSRifleWeapon::AutoFire()
{
	if (!GetWorld() || IsBulletEmpty()) { return; }

	if (!GetWorld()->GetTimerManager().IsTimerActive(AutoFireTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimer, this, &ATPSRifleWeapon::SemiFire, FireRate, true);
	}
}


bool ATPSRifleWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd, float HalfConeAng) const
{
	return Super::GetTraceData(TraceStart, TraceEnd, HalfConeAng);
}

void ATPSRifleWeapon::MakeShot()
{
	UE_LOG(MyATPSRifleWeaponLog, Error, TEXT("Fire"));
	if (!GetWorld() || IsBulletEmpty()) { return; }
	BIsUnderFire = true;


	FVector StartLocation;
	FVector EndLocation;
	if (!GetTraceData(StartLocation, EndLocation, BulletSpread)) { return; }


	MakeTraceHit(LineTraceHitResult, StartLocation, EndLocation);

	// 击中则画射线(只有当枪口的前向量 和枪口点到击中点这条向量 的点积 角度小于90才绘制射线)
	if (LineTraceHitResult.bBlockingHit && GetAngleBetweenMuzzleAndHitPoint(
		GetMuzzleWorldTransform(), LineTraceHitResult) <= 90.0f)
	{

		// 绘制枪口射线 
		// DrawDebugLine(GetWorld(), GetMuzzleWorldTransform().GetLocation(), LineTraceHitResult.ImpactPoint, FColor::Red,
		//               false, 2,
		//               0, 3);

		// DrawDebugSphere(GetWorld(), LineTraceHitResult.ImpactPoint, 5.f, 5, FColor::Red, false, 3.f, 0, 3.f);
		// 打印被击中的骨骼名字
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
		                                 FString::Printf(TEXT("Hit Bone: %s"), *LineTraceHitResult.BoneName.ToString()),
		                                 false, FVector2D(2, 2));

		MakeDamage(LineTraceHitResult);
		WeaponFXComponent->PlayImpactFX(LineTraceHitResult);
	}
	else
	{

		// 绘制摄像机射线
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Yellow, false, 3.f, 0, 3);
	}
	DecreaseBullet();
}

void ATPSRifleWeapon::MakeDamage(const FHitResult &HitResult)
{
	if (!HitResult.GetActor()) { return; }
	if (ATPSBaseCharacter *AtpsBaseCharacter = Cast<ATPSBaseCharacter>(HitResult.GetActor()))
	{
		AtpsBaseCharacter->TakeDamage(WeaponDamage, FDamageEvent{}, GetPlayerController(), this);
	}
}
