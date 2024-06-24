// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/TPSBaseCharacter.h"
#include "TPSAICharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;
/**
 * AI角色类,继承自玩家的基础角色类
 */
UCLASS()
class FUTURETPS_API ATPSAICharacter : public ATPSBaseCharacter
{
	GENERATED_BODY()

public:
	// 调用父类构造器,(父类构造器通过这种构造器重写了角色移动组件,这里我们重写父类的武器逻辑组件)
	ATPSAICharacter(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AI)
	UBehaviorTree *BehaviorTreeAsset;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UWidgetComponent *HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	float HealthBarVisibilityDistance = 2800.f;

	virtual void BeginPlay() override;

	virtual void OnDeath() override;

	virtual void OnHealthChanged(float Health, bool BIsDecreaseHealth) override;

	/// 根据当前AI和玩家的距离,设置AI生命值UI的可见性
	void UpdateHealthBarWidgetVisibility() const;
};
