// FutureTPS Game All Rights Reserved


#include "UI/TPSPlayerHUDWidget.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "Components/TPSHealthComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSPlayerHUDWidgetLog, All, All);

bool UTPSPlayerHUDWidget::Initialize()
{
	if (GetOwningPlayer())
	{
		// 绑定委托
		// 当AController占据一个新的Pawn时触发的回调函数
		// 也就是在APawn::OnPossess()被调用后(并且possess了一个新Pawn)就会触发我们的回调函数(OnNewPawn)
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UTPSPlayerHUDWidget::OnNewPawn);

		// 首次需要手动调用一次,因为控制器的OnPossess函数在 本类的Initialize之前调用(可以通过覆写OnPossess打日志来验证)
		OnNewPawn(GetOwningPlayer()->GetPawn());
	}
	return Super::Initialize();
}

float UTPSPlayerHUDWidget::GetHealthPercent() const
{
	const UTPSHealthComponent *HealthComponent = FTPSUtils::GetComponentByCurrentPlayer<UTPSHealthComponent>(
		GetOwningPlayerPawn());

	if (!HealthComponent) { return 0.0f; }
	return HealthComponent->GetHealthPercent();
}

bool UTPSPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData &WeaponUIData) const
{
	const UTPSWeaponLogicComponent *WeaponLogicComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSWeaponLogicComponent>(GetOwningPlayerPawn());
	if (!WeaponLogicComponent) { return false; }


	return WeaponLogicComponent->GetWeaponUIData(WeaponUIData);
}

bool UTPSPlayerHUDWidget::GetCurrentWeaponAmmo(FAmmoData &AmmoData) const
{
	const UTPSWeaponLogicComponent *WeaponLogicComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSWeaponLogicComponent>(GetOwningPlayerPawn());

	if (!WeaponLogicComponent) { return false; }


	return WeaponLogicComponent->GetWeaponAmmo(AmmoData);
}

FText UTPSPlayerHUDWidget::GetCurrentAmmoText() const
{

	FAmmoData AmmoData;
	if (!GetCurrentWeaponAmmo(AmmoData)) { return FText::FromString(""); }

	const FString String = FString::FromInt(AmmoData.Bullets);

	if (AmmoData.InfiniteMag) { return FText::FromString(String + "/" + TEXT("∞")); }

	return FText::FromString(String + "/" + FString::FromInt(AmmoData.Magazines));
}


bool UTPSPlayerHUDWidget::IsPlayerAlive() const
{
	const UTPSHealthComponent *HealthComponent = FTPSUtils::GetComponentByCurrentPlayer<UTPSHealthComponent>(
		GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UTPSPlayerHUDWidget::IsPlayerSpectating() const
{
	// 获取当前玩家controller
	const APlayerController *PlayerController = GetOwningPlayer();

	return PlayerController && PlayerController->GetStateName() == NAME_Spectating;
}

ESlateVisibility UTPSPlayerHUDWidget::SettingPlayerVisibility() const
{
	if (IsPlayerAlive()) { return ESlateVisibility::Visible; }


	return ESlateVisibility::Hidden;
}

ESlateVisibility UTPSPlayerHUDWidget::SettingSpectatorVisibility() const
{
	if (!IsPlayerSpectating()) { return ESlateVisibility::Hidden; }
	return ESlateVisibility::Visible;
}

ESlateVisibility UTPSPlayerHUDWidget::IsReloading() const
{
	const UTPSWeaponLogicComponent *WeaponLogicComponent = FTPSUtils::GetComponentByCurrentPlayer<
		UTPSWeaponLogicComponent>(GetOwningPlayerPawn());

	if (!WeaponLogicComponent || !WeaponLogicComponent->IsReloading()) { return ESlateVisibility::Hidden;; }

	return ESlateVisibility::Visible;
}

void UTPSPlayerHUDWidget::HealthChanged(float Health, bool BIsDecreaseHealth)
{
	OnHealthChanged(Health, BIsDecreaseHealth);
}

void UTPSPlayerHUDWidget::OnNewPawn(APawn *NewPawn)
{
	if (!NewPawn) { UE_LOG(MyUTPSPlayerHUDWidgetLog, Error, TEXT("NewPawn Cant be NUll!")); }
	UTPSHealthComponent *HealthComponent = FTPSUtils::GetComponentByCurrentPlayer<UTPSHealthComponent>(NewPawn);
	if (HealthComponent)
	{
		// 绑定生命组件的委托 (收到伤害时的委托)
		HealthComponent->OnHealthChanged.AddUObject(this, &UTPSPlayerHUDWidget::HealthChanged);
	}

}
