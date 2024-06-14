// FutureTPS Game All Rights Reserved


#include "Weapons/TPSBaseWeapon.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSBaseWeaponLog, All, All)

// Sets default values
ATPSBaseWeapon::ATPSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponSkeletalMeshComponent);
}

void ATPSBaseWeapon::Fire() { BIsUnderFire = true; }

void ATPSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponSkeletalMeshComponent)
	checkf(DefaultAmmo.Bullets >= 0, TEXT("The default number of bullets cannot be less than 0"));
	checkf(DefaultAmmo.Magazines >= 0, TEXT("The default number of magazines cannot be less than 0"));

	CurrentAmmo = DefaultAmmo;
}

APlayerController *ATPSBaseWeapon::GetPlayerController() const
{

	ACharacter *Player = Cast<ACharacter>(GetOwner());
	if (!Player) { return nullptr; }

	return Player->GetController<APlayerController>();
}

bool ATPSBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const
{
	// ViewLocation摄像机的view点,就是摄像机的正中心
	// ViewRotation可以通过摄像机的rotation获取方向向量


	// 判断当前控制的人是AI还是玩家
	const auto TPSCharacter = Cast<ACharacter>(GetOwner());
	if (TPSCharacter->IsPlayerControlled())
	{
		APlayerController *PlayerController = GetPlayerController();

		// 获取摄像机视点,用来纠正射击轨道 (即子弹是从枪口射到摄像机的准星中间)
		if (!PlayerController) { return false; }

		PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
		return true;
	}
	else
	{
		// 如果是AI,那么返回的坐标是以枪口点进行射击
		ViewLocation = GetMuzzleWorldTransform().GetLocation();
		ViewRotation = GetMuzzleWorldTransform().Rotator();
	}

	return true;
}

FTransform ATPSBaseWeapon::GetMuzzleWorldTransform() const
{
	return WeaponSkeletalMeshComponent->GetSocketTransform(MuzzlePosition);
}

bool ATPSBaseWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd, float HalfConeAng) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) { return false; }


	TraceStart = ViewLocation; // 射线起点等于摄像机的中点(玩家视角的中点)
	FVector Direction = ViewRotation.Vector(); // 摄像机的射线前方向向量

	// 射线终点
	TraceEnd = TraceStart + ((HalfConeAng > 0.0f)
		? FMath::VRandCone(Direction, FMath::DegreesToRadians(HalfConeAng)) // 锥度不为0,则以Direction方向为基础方向生成锥度范围内的射线方向
		: Direction) * TraceLength;

	return true;
}

void ATPSBaseWeapon::MakeTraceHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd) const
{
	if (!GetWorld()) { return; }

	FCollisionQueryParams CollisionParam;
	// 射线会忽略当前武器的actor,不会击中自己
	CollisionParam.AddIgnoredActor(GetOwner());

	// 允许返回的HitResult拿到击中物体的物理材质
	CollisionParam.bReturnPhysicalMaterial = true;

	// 发出射线检测碰撞(起点是摄像机的屏幕中心)，相关碰撞信息保存在HitResult
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParam);
}


float ATPSBaseWeapon::
GetAngleBetweenMuzzleAndHitPoint(const FTransform &MuzzleTransform, const FHitResult &Hit)
{
	// 枪口向量的前向量
	FVector a = MuzzleTransform.GetRotation().GetForwardVector();

	// 获得枪口点位置到冲击点的 这条向量
	FVector b = (Hit.ImpactPoint - MuzzleTransform.GetLocation()).GetSafeNormal();

	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(a, b)));
}

void ATPSBaseWeapon::SemiFire() {}

void ATPSBaseWeapon::StopFire() { BIsUnderFire = false; }

void ATPSBaseWeapon::Reload() { ChangeMagz(); }

void ATPSBaseWeapon::AutoFire() {}

void ATPSBaseWeapon::MakeShot() { BIsUnderFire = true; }

void ATPSBaseWeapon::DecreaseBullet()
{
	CurrentAmmo.Bullets = FMath::Clamp(CurrentAmmo.Bullets - 1, 0, CurrentAmmo.Bullets);
}

void ATPSBaseWeapon::ChangeMagz()
{
	if (IsMagzEmpty()) { return; }

	if (!IsMagzEmpty() && !CurrentAmmo.InfiniteMag)
	{
		CurrentAmmo.Magazines = FMath::Clamp(CurrentAmmo.Magazines - 1, 0, CurrentAmmo.Magazines);
	}

	CurrentAmmo.Bullets = DefaultAmmo.Bullets;


	// UE_LOG(MyATPSBaseWeaponLog, Error, TEXT("---ChangeMagz!---"));
}

bool ATPSBaseWeapon::CanReload() const { return CurrentAmmo.Bullets < DefaultAmmo.Bullets && !IsMagzEmpty(); }

void ATPSBaseWeapon::Resupply(float ResupplyPercent)
{
	if (ResupplyPercent <= 0) { return; }
	const int32 TotalBullets = FMath::RoundToInt32((DefaultAmmo.Magazines * DefaultAmmo.Bullets) * ResupplyPercent);
	const int32 Magz = TotalBullets / DefaultAmmo.Bullets;

	if (CurrentAmmo.Magazines < DefaultAmmo.Magazines)
	{
		CurrentAmmo.Magazines = FMath::Clamp(CurrentAmmo.Magazines + Magz, CurrentAmmo.Magazines,
		                                     DefaultAmmo.Magazines);
	}
	else
	{
		CurrentAmmo.Bullets = FMath::Clamp(CurrentAmmo.Bullets + DefaultAmmo.Bullets, CurrentAmmo.Bullets,
		                                   DefaultAmmo.Bullets);
	}

	UE_LOG(MyATPSBaseWeaponLog, Error, TEXT("[ResupplyMagz]TotalBullets: %d , Magz: %d "), TotalBullets, Magz);
}

bool ATPSBaseWeapon::IsFullAmmo() const
{
	return CurrentAmmo.Bullets == DefaultAmmo.Bullets && CurrentAmmo.Magazines == DefaultAmmo.Magazines;
}

bool ATPSBaseWeapon::IsBulletEmpty() const { return CurrentAmmo.Bullets == 0; }

bool ATPSBaseWeapon::IsMagzEmpty() const { return CurrentAmmo.Magazines == 0; }

void ATPSBaseWeapon::DebugLogAmmo() const
{
	FString AmmoInfo = "Bullets : " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += (CurrentAmmo.InfiniteMag) ? "InfiniteMagz" : "Magazines : " + FString::FromInt(CurrentAmmo.Magazines);
	UE_LOG(MyATPSBaseWeaponLog, Error, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent *ATPSBaseWeapon::SpawnMuzzleFXComponent()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponSkeletalMeshComponent, MuzzlePosition,
	                                                    FVector::Zero(),
	                                                    FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget,
	                                                    true);
}
