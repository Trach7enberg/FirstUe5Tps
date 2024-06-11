// FutureTPS Game All Rights Reserved
DEFINE_LOG_CATEGORY_STATIC(MyUTPSHealthComponentLog, All, All)

#include "Components/TPSHealthComponent.h"

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
	Health = FMath::Clamp(value, Health, MaxHealth);

	// 当血量改变时才更新血量文本显示,在character类里绑定回调函数,该委托一广播就会调用:更新文本显示的那个回调函数
	OnHealthChanged.Broadcast(Health);
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

	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

	// 现在生命值减少,所以广播通知绑定了该委托的回调函数进行调用
	OnHealthChanged.Broadcast(Health);
	// UE_LOG(MyUTPSHealthComponentLog, Error, TEXT("Hurted"));
	// 处理是否自动回血
	NeedHeal.Broadcast();

	if (IsDead())
	{
		// 死亡则广播,所有接到通知并且已经绑定回调函数的将会调用回调函数处理
		OnDeath.Broadcast();
	}
}
