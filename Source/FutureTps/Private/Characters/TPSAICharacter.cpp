// FutureTPS Game All Rights Reserved


#include "Characters/TPSAICharacter.h"

#include "BrainComponent.h"
#include "Components/TPSAIWeaponLogicComponent.h"
#include "Components/TPSHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "UI/Widgets/TPSHealthBarWidget.h"
#include "controllers/TPSAIController.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSAICharacterLog, All, All);

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer &ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTPSAIWeaponLogicComponent>("WeaponLogicComponent"))
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ATPSAICharacter::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		// 设置AI转向的丝滑度
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// 注意是YAW!! Z 对应 YAW
		GetCharacterMovement()->RotationRate = FRotator(0, 200.0f, 0);
	}

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBar");;
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 89));

}

void ATPSAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthBarWidgetVisibility();
}

void ATPSAICharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthBarWidgetComponent);
}

void ATPSAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto TPSAiController = Cast<AAIController>(GetController());
	if (TPSAiController && TPSAiController->BrainComponent)
	{
		// BrainComponent是 底层的行为树组件,行为树组件通过它来Cast的,所以我们调用Cleanup函数
		// 当AI死亡时关闭行为树
		TPSAiController->BrainComponent->Cleanup();
		// UE_LOG(MyATPSAICharacterLog,Error,TEXT("CleanUpBehaviorTree"));
	}

}

void ATPSAICharacter::OnHealthChanged(float Health, bool BIsDecreaseHealth)
{
	Super::OnHealthChanged(Health, BIsDecreaseHealth);

	const auto HealthBarWidget = Cast<UTPSHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget) { return; }

	HealthBarWidget->SetHealthBar(HealthComponent->GetHealthPercent());
}

void ATPSAICharacter::UpdateHealthBarWidgetVisibility() const
{
	if (!GetWorld() || !GetWorld()->GetControllerIterator()) { return; }

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (!It->Get()->IsPlayerController()) { continue; }


		const auto PlayerController = Cast<APlayerController>(It->Get());

		if (!PlayerController || !PlayerController->GetPawnOrSpectator()) { continue; }
		const auto PlayerLocation = PlayerController->GetPawnOrSpectator()->GetActorLocation();
		const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

		// TODO 多人游戏时,需要根据玩家的位置来设置AI的生命值UI的可见性
		HealthBarWidgetComponent->SetVisibility(Distance < HealthBarVisibilityDistance, true);
	}

}
