// FutureTPS Game All Rights Reserved


DEFINE_LOG_CATEGORY_STATIC(MyUTPSWeaponLogicComponentLog, All, All)

#include "Components/TPSWeaponLogicComponent.h"
#include "Weapons/TPSBaseWeapon.h"
#include "Animations/MyAnimUtils.h"
#include "GameFramework/Character.h"
#include "Animations/AnimNotifys/TPSEquipBeginAnimNotify.h"
#include "Animations/AnimNotifys/TPSEquipFinishedAnimNotify.h"
#include "Animations/AnimNotifys/TPSReloadBeginAnimNotify.h"
#include "Animations/AnimNotifys/TPSReloadFinishedAnimNotify.h"

UTPSWeaponLogicComponent::UTPSWeaponLogicComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	FString EquipName = TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Animations/AM_Equip.AM_Equip'");


	// 设置切换蒙太奇的动画实例
	if (UAnimMontage *EquipAnimMontageBp = LoadObject<UAnimMontage>(
		nullptr, *EquipName))
	{
		// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("GetAnimMontage"));
		EquipAnimMontage = EquipAnimMontageBp;
	}

}


// Called when the game starts
void UTPSWeaponLogicComponent::BeginPlay()
{
	Super::BeginPlay();
	// 绑定 当切枪时又换弹的委托 调用的回调函数
	OnSwitchWhenChangeMagz.AddUObject(this, &UTPSWeaponLogicComponent::ResetReloadState);

	InitWeaponData();

	checkf(Weapons.Num()>=2, TEXT("The number of weapons must be at least 2 (for now)"));
	// 由于生成武器数组时,武器都绑定到背部了,所以开始游戏时需要将一把武器帮到手上
	EquipWeapon(0);

}

void UTPSWeaponLogicComponent::Fire()
{

	if (!CanFire()) { return; }
	CurrentWeapon->Fire();
}

void UTPSWeaponLogicComponent::StopFire()
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->StopFire();

}


void UTPSWeaponLogicComponent::Reload()
{

	if (!CurrentWeapon || !CurrentReloadAnimMontage || !CanReload()) { return; }
	if (IsUnderFire()) { StopFire(); }
	// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("InReload!"));
	PlayAnimMontage(CurrentReloadAnimMontage);
	// CurrentWeapon->Reload();
	// 帧动画完成才会reload,实际reload在回下面调函数里

}

void UTPSWeaponLogicComponent::Resupply(float ResupplyPercent) const
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->Resupply(ResupplyPercent);
}

bool UTPSWeaponLogicComponent::GetWeaponUIData(FWeaponUIData &WeaponUIData) const
{
	if (!CurrentWeapon) { return false; }

	WeaponUIData = CurrentWeapon->GetWeaponUIData();

	return true;
}

bool UTPSWeaponLogicComponent::GetWeaponAmmo(FAmmoData &AmmoData) const
{
	if (!CurrentWeapon) { return false; }

	AmmoData = CurrentWeapon->GetCurrentAmmo();
	return true;
}


bool UTPSWeaponLogicComponent::CanFire()
{

	// 当角色切换武器、换弹中、都不能开枪 //TODO 冲刺时不能开枪
	if (!CurrentWeapon || !CanSwitchWeapon() || IsReloading()) { return false; }

	return true;
}

bool UTPSWeaponLogicComponent::CanReload() const
{
	if (!CurrentWeapon || IsReloading()) { return false; }
	return CurrentWeapon->CanReload();
}

bool UTPSWeaponLogicComponent::IsReloading() const { return BisBeginWeaponReloading; }

bool UTPSWeaponLogicComponent::IsFullAmmo() const
{
	if (!CurrentWeapon) { return false; }

	return CurrentWeapon->IsFullAmmo();
}

bool UTPSWeaponLogicComponent::IsEmptyAmmo() const
{
	if (!CurrentWeapon) { return false; }
	return CurrentWeapon->IsEmptyAmmo();
}

bool UTPSWeaponLogicComponent::IsUnderFire() const
{
	if (!CurrentWeapon) { return false; }
	return CurrentWeapon->IsUnderFire();
}

void UTPSWeaponLogicComponent::ResetReloadState()
{
	BisBeginWeaponReloading = false;
	BisCompleteWeaponReloading = true;
	// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("SwitchWhenChangeMagz!"));
}

bool UTPSWeaponLogicComponent::CanSwitchWeapon() const
{
	return !BIsBeginWeaponSwitching && BIsCompleteWeaponSwitching;
}

ACharacter *UTPSWeaponLogicComponent::GetCharacter(AActor *Owner) { return Cast<ACharacter>(Owner); }

void UTPSWeaponLogicComponent::InitWeaponData()
{
	// 通过GetOwner获取Character,因为我们的组件是附在Character下的,所以是获取Character

	ACharacter *Character = GetCharacter(GetOwner());
	if (!GetWorld() || !Character || !EquipAnimMontage) { return; }
	if (WeaponData.Num() == 0)
	{
		UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("Equip anim notify shouldn't frogotten to setting"));
		checkNoEntry();
	}

	// 给切换武器的动画绑定动画notify回调函数
	MyAnimUtils::InitAnimationNotifies<UTPSEquipBeginAnimNotify, UTPSEquipFinishedAnimNotify, UTPSWeaponLogicComponent
	                                   , &UTPSWeaponLogicComponent::OnEquipBegin, &
	                                   UTPSWeaponLogicComponent::OnEquipFinished>(EquipAnimMontage, this);


	for (auto Element : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ATPSBaseWeapon>(Element.WeaponClasses);
		if (!Weapon || !Element.ReloadAnimMontage) { continue; }

		MyAnimUtils::InitAnimationNotifies<UTPSReloadBeginAnimNotify, UTPSReloadFinishedAnimNotify,
		                                   UTPSWeaponLogicComponent
		                                   , &UTPSWeaponLogicComponent::OnReloadBegin, &
		                                   UTPSWeaponLogicComponent::OnReloadFinished>(Element.ReloadAnimMontage, this);
		// 设置武器的所有者是当前的character,以便我们可以通过武器获取所有者进行相关操作
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		// 初始化时,统一将武器绑定到背上,之后选其中一把绑定到手上
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("Weapons: %d"), Weapons.Num());

}

void UTPSWeaponLogicComponent::AttachWeaponToSocket(ATPSBaseWeapon *Weapon, USceneComponent *CharacterMesh,
                                                    const FName &SocketName)
{
	if (!Weapon || !CharacterMesh) { return; }

	// 因为武器逻辑组件是挂在我们的character下的,所以转换成ACharacter
	FAttachmentTransformRules AttachmentRules{EAttachmentRule::SnapToTarget, false};
	Weapon->AttachToComponent(CharacterMesh, AttachmentRules, SocketName);
}

void UTPSWeaponLogicComponent::EquipWeapon(int32 WeaponIndex)
{
	ACharacter *Character = GetCharacter(GetOwner());

	// 当前武器等于要切换的武器也是不能切换的
	if (!Character || WeaponIndex < 0 || WeaponIndex >= Weapons.Num() || CurrentWeapon == Weapons[WeaponIndex])
	{
		return;
	}

	PlayAnimMontage(EquipAnimMontage);

	// 手中存在武器,需要放到背上
	if (CurrentWeapon) { AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName); }

	CurrentWeapon = Weapons[WeaponIndex];

	// 在武器数组里找出与当前已经生成的武器相同的对象,然后设置换弹动画
	auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData &Data)
	{
		return Data.WeaponClasses == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimMontage = (CurrentWeaponData) ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

// TODO 当跳跃切枪时候,跳跃的Start动画会再次触发,待修复
void UTPSWeaponLogicComponent::SwitchWeapon()
{

	if (Weapons.Num() == 0 || !CanSwitchWeapon()) { return; }


	// 切换武器时如果手中有武器那么要停火
	if (IsUnderFire()) { CurrentWeapon->StopFire(); }

	// 当换弹时又切枪时,重置换弹动画的通知
	if (IsReloading()) { OnSwitchWhenChangeMagz.Broadcast(); }

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();


	EquipWeapon(CurrentWeaponIndex);
}


void UTPSWeaponLogicComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}

	Super::EndPlay(EndPlayReason);
}

void UTPSWeaponLogicComponent::PlayAnimMontage(UAnimMontage *Am)
{
	ACharacter *Character = GetCharacter(GetOwner());
	if (!Character) { return; }

	Character->PlayAnimMontage(Am);
}


void UTPSWeaponLogicComponent::OnEquipFinished(USkeletalMeshComponent *SkeletalMeshComponent)
{
	ACharacter *Character = GetCharacter(GetOwner());
	if (!Character) { return; }

	// 动画蒙太奇通知的所属SkeletalMesh是我们玩家所控制的Character的那个SkeletalMesh时才是正确的,需要过滤一下
	if (SkeletalMeshComponent == Character->GetMesh())
	{
		// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("FinishedSwitching!"));
		BIsBeginWeaponSwitching = false;
		BIsCompleteWeaponSwitching = true;
	}
}

void UTPSWeaponLogicComponent::OnEquipBegin(USkeletalMeshComponent *SkeletalMeshComponent)
{
	ACharacter *Character = GetCharacter(GetOwner());
	if (!Character) { return; }

	// 动画蒙太奇通知的所属SkeletalMesh是我们玩家所控制的Character的那个SkeletalMesh时才是正确的,需要过滤一下
	if (SkeletalMeshComponent == Character->GetMesh())
	{
		// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("BeginSwitching!"));
		BIsBeginWeaponSwitching = true;
		BIsCompleteWeaponSwitching = false;
	}
}

void UTPSWeaponLogicComponent::OnReloadBegin(USkeletalMeshComponent *SkeletalMeshComponent)
{
	ACharacter *Character = GetCharacter(GetOwner());
	if (!Character) { return; }
	if (SkeletalMeshComponent == Character->GetMesh())
	{
		BisBeginWeaponReloading = true;
		BisCompleteWeaponReloading = false;
		// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("ReloadBegin!"));
	}
}

void UTPSWeaponLogicComponent::OnReloadFinished(USkeletalMeshComponent *SkeletalMeshComponent)
{
	ACharacter *Character = GetCharacter(GetOwner());
	if (!Character || !CurrentWeapon) { return; }
	if (SkeletalMeshComponent == Character->GetMesh())
	{
		CurrentWeapon->Reload();
		BisBeginWeaponReloading = false;
		BisCompleteWeaponReloading = true;
		// UE_LOG(MyUTPSWeaponLogicComponentLog, Error, TEXT("ReloadFinished!"));
	}
}
