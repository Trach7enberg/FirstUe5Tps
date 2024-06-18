// FutureTPS Game All Rights Reserved


#include "Components/TPSHealthComponent.h"


DEFINE_LOG_CATEGORY_STATIC(MyUTPSHealthComponentLog, All, All)

// Sets default values for this component's properties
UTPSHealthComponent::UTPSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 绑定当受到任何伤害时回调的函数
	if (AActor *Owner = GetOwner()) { Owner->OnTakeAnyDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeAnyDamage); }

	// 添加绑定自动回血委托
	NeedHeal.AddUObject(this, &UTPSHealthComponent::HealHandle);
}


// Called when the game starts
void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	check(MaxHealth > 0);
	Health = MaxHealth;
}

void UTPSHealthComponent::SetHealth(float value)
{
	if (IsDead()) { return; }
	Health = FMath::Clamp(value, 0, MaxHealth);

	// 当血量改变时才更新血量文本显示,在character类里绑定回调函数,该委托一广播就会调用:更新文本显示的那个回调函数
	// 生命值增加中所以是false
	OnHealthChanged.Broadcast(Health, false);
}

void UTPSHealthComponent::AutoHeal()
{
	if (FMath::IsNearlyEqual(Health, MaxHealth) || IsDead() || !BisAutoHealth || !GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
		return;
	}
	SetHealth(Health + HealModifier);
}

void UTPSHealthComponent::HealHandle()
{
	if (IsDead() || !GetWorld()) { return; }
	if (BisAutoHealth && !GetWorld()->GetTimerManager().IsTimerActive(HealTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UTPSHealthComponent::AutoHeal, HealUpdateTime,
		                                       true,
		                                       HealDelay);
	}
}

void UTPSHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                          AController *InstigatedBy, AActor *DamageCauser)
{
	if (Damage <= 0 || IsDead()) { return; }

	SetHealth(Health - Damage);

	// 现在生命值减少 所以是true,所以广播通知绑定了该委托的回调函数进行调用
	OnHealthChanged.Broadcast(Health, true);
	// UE_LOG(MyUTPSHealthComponentLog, Error, TEXT("Hurted: %f"), Damage);
	// 处理是否自动回血	
	NeedHeal.Broadcast();

	OscillationCamera();

	if (IsDead())
	{
		// 死亡则广播,所有接到通知并且已经绑定回调函数的将会调用回调函数处理
		OnDeath.Broadcast();
	}
}


void UTPSHealthComponent::OscillationCamera() const
{
	if (IsDead() || !CameraShake) { return; }

	// 获取APawn,GetOwner() 是 character,APawn的子类 
	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) { return; }

	// APawn里获取玩家控制器的指针,再通过控制器获取控制的角色身上的相机播放抖动
	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) { return; }

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
