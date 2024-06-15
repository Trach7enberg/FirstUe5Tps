// FutureTPS Game All Rights Reserved


#include "Components/TPSAIWeaponLogicComponent.h"

#include "Weapons/TPSBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSAIWeaponLogicComponentLog, All, All);

void UTPSAIWeaponLogicComponent::Fire()
{
	if (!CanFire()) { return; }

	if (CurrentWeapon->IsEmptyAmmo()) { SwitchWeapon(); }
	else if (CurrentWeapon->IsBulletEmpty()) { Reload(); }
	else { CurrentWeapon->Fire(); }

}

void UTPSAIWeaponLogicComponent::SwitchWeapon()
{
	if (Weapons.Num() == 0 || !CanSwitchWeapon()) { return; }


	// 切换武器时如果手中有武器那么要停火
	if (IsUnderFire()) { CurrentWeapon->StopFire(); }

	// 当换弹时又切枪时,重置换弹动画的通知
	if (IsReloading()) { OnSwitchWhenChangeMagz.Broadcast(); }

	int32 Index;
	// 切换武器时 遍历所有武器找有子弹的武器切换 TODO 当武器全没有子弹时 不切枪或者切刀
	for (Index = (CurrentWeaponIndex + 1) % Weapons.Num();
	     Index != CurrentWeaponIndex;
	     Index = (Index + 1) % Weapons.Num()) { if (!Weapons[Index]->IsEmptyAmmo()) { break; } }

	// 不等于当前武器才能换武器,否则退出,不然进入死循环
	if (Index != CurrentWeaponIndex) { CurrentWeaponIndex = Index; }

	EquipWeapon(CurrentWeaponIndex);
}
