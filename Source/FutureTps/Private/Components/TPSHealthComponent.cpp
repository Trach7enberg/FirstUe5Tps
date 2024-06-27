// FutureTPS Game All Rights Reserved


#include "Components/TPSHealthComponent.h"

#include "GameFramework/Character.h"
#include "GameMode/TPSGameModeBase.h"
#include "Perception/AISense_Damage.h"


DEFINE_LOG_CATEGORY_STATIC(MyUTPSHealthComponentLog, All, All)

// Sets default values for this component's properties
UTPSHealthComponent::UTPSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 绑定当受到伤害时回调的函数
	if (AActor *Owner = GetOwner())
	{
		// Owner->OnTakeAnyDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeAnyDamage);
		Owner->OnTakePointDamage.AddDynamic(this, &UTPSHealthComponent::OnTakePointDamage);
		Owner->OnTakeRadialDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeRadialDamage);
	}

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

void UTPSHealthComponent::ApplyDamage(float Damage, AController *InstigatedBy)
{
	if (Damage <= 0 || IsDead()) { return; }

	SetHealth(Health - Damage);

	// 现在生命值减少 所以是true,所以广播通知绑定了该委托的回调函数进行调用
	OnHealthChanged.Broadcast(Health, true);
	// UE_LOG(MyUTPSHealthComponentLog, Error, TEXT("Hurted: %f"), Damage);
	// 处理是否自动回血	
	NeedHeal.Broadcast();

	OscillationCamera();

	// 扣血之后判断是否死亡
	if (IsDead())
	{
		// InstigatedBy是引发伤害的Controller
		// 调用KillPlayer函数记录相应的PlayerState击杀死亡信息
		KillPlayer(InstigatedBy);

		// 死亡则广播,所有接到通知并且已经绑定回调函数的将会调用回调函数处理
		OnDeath.Broadcast();
	}

	ReportDamageEvent(Damage, InstigatedBy);
}

void UTPSHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                          AController *InstigatedBy, AActor *DamageCauser)
{
	UE_LOG(MyUTPSHealthComponentLog, Warning, TEXT("AnyDamage! Damaged:%.1f"), Damage);
}

void UTPSHealthComponent::OnTakePointDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
                                            FVector HitLocation, UPrimitiveComponent *FHitComponent, FName BoneName,
                                            FVector ShotFromDirection,
                                            const UDamageType *DamageType, AActor *DamageCauser)
{
	float FinalDamage = Damage * GetPointDamagedModifiers(BoneName, DamagedActor);
	UE_LOG(MyUTPSHealthComponentLog, Warning,
	       TEXT("PointDamage! Damaged:%.1f , Bone:%s, Multiple:%.1f , FinalDamaged:%.1f"), Damage,
	       *BoneName.ToString(), GetPointDamagedModifiers(BoneName,DamagedActor), FinalDamage);
	ApplyDamage(FinalDamage, InstigatedBy);
}

void UTPSHealthComponent::OnTakeRadialDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                             FVector Origin, const FHitResult &HitInfo, AController *InstigatedBy,
                                             AActor *DamageCauser)
{
	UE_LOG(MyUTPSHealthComponentLog, Warning, TEXT("RadialDamage! Damaged:%.1f"), Damage);
	ApplyDamage(Damage, InstigatedBy);
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

void UTPSHealthComponent::KillPlayer(AController *Killer)
{
	if (!GetWorld()) { return; }

	const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) { return; }

	const auto VictimPlayer = Cast<APawn>(GetOwner());
	const auto VictimController = VictimPlayer ? VictimPlayer->GetController() : nullptr;

	GameMode->KillPlayer(Killer, VictimController);
}

float UTPSHealthComponent::GetPointDamagedModifiers(const FName &BoneName, AActor *DamagedActor) const
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if (!Character || DamagedModifiers.Num() == 0 || !Character->GetMesh() || !Character->GetMesh()->
		GetBodyInstance(BoneName)) { return 1.0f; }

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysMaterial || !DamagedModifiers.Contains(PhysMaterial)) { return 1.0f; }


	return DamagedModifiers[PhysMaterial];

}

void UTPSHealthComponent::ReportDamageEvent(const float Damage, const AController *InstigatedBy) const
{
	if (!GetWorld() || !GetOwner() || !InstigatedBy || !InstigatedBy->GetPawn()) { return; }

	UE_LOG(MyUTPSHealthComponentLog,Warning,TEXT("Report!"));
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage,
	                                   InstigatedBy->GetPawn()->GetActorLocation(),
	                                   GetOwner()->GetActorLocation());
}
