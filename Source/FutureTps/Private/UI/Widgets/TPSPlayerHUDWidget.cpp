// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSPlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "Components/TPSHealthComponent.h"
#include "TPSUtil/TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSPlayerHUDWidgetLog, All, All);

void UTPSPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GetOwningPlayer())
	{
		// 绑定委托
		// 当AController占据一个新的Pawn时触发的回调函数
		// 也就是在APawn::OnPossess()被调用后(并且possess了一个新Pawn)就会触发我们的回调函数(OnNewPawn)
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UTPSPlayerHUDWidget::OnNewPawn);

		// 首次需要手动调用一次,因为控制器的OnPossess函数在 本类的Initialize之前调用(可以通过覆写OnPossess打日志来验证)
		OnNewPawn(GetOwningPlayer()->GetPawn());
	}

	if (PlayerHealthBar) { PlayerHealthBar->SetFillColorAndOpacity(FullHealthColor); }
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

FText UTPSPlayerHUDWidget::GetCurrentBulletsText() const
{

	FAmmoData AmmoData;
	if (!GetCurrentWeaponAmmo(AmmoData)) { return FText::FromString(""); }


	return FText::FromString(FString::FromInt(AmmoData.Bullets));
}

FText UTPSPlayerHUDWidget::GetCurrentMagzsText() const
{
	FAmmoData AmmoData;
	if (!GetCurrentWeaponAmmo(AmmoData)) { return FText::FromString(""); }


	return FText::FromString(AmmoData.InfiniteMag ? TEXT("∞") : FString::FromInt(AmmoData.Magazines));
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
	UpdatePlayerHealthBar();

	// 播放角色受击动画
	if (!IsAnimationPlaying(BloodBlinking) && BIsDecreaseHealth)
	{
		PlayAnimation(BloodBlinking, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true);
	}

}

void UTPSPlayerHUDWidget::OnNewPawn(APawn *NewPawn)
{
	if (!NewPawn) { return; }

	UTPSHealthComponent *HealthComponent = FTPSUtils::GetComponentByCurrentPlayer<UTPSHealthComponent>(NewPawn);
	if (!HealthComponent) { return; }

	// 绑定生命组件的委托 (收到伤害时的委托)
	HealthComponent->OnHealthChanged.AddUObject(this, &UTPSPlayerHUDWidget::HealthChanged);


	// 首次进入游戏、复活时,需要更新一下生命值UI
	// 由于有时候GetHealthPercent()会返回0,所以这里要判断一下角色是否死亡,\\ TODO 不然会老是把生命值UI显示为0( 莫名其妙的bug,待查)
	if (!HealthComponent->IsDead()) { UpdatePlayerHealthBar(); }

}

void UTPSPlayerHUDWidget::UpdatePlayerHealthBar() const
{
	if (!PlayerHealthBar) { return; }

	const auto Percent = GetHealthPercent();


	const auto Color = (Percent <= BarColorChangePercent)
		? LowHealthColor
		: FullHealthColor;

	PlayerHealthBar->SetFillColorAndOpacity(Color);
	PlayerHealthBar->SetPercent(Percent);
}
