// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSHealthBarWidget.generated.h"

class UProgressBar;
/**
 * 生命值UI
 */
UCLASS()
class FUTURETPS_API UTPSHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/// 设置生命值百分比、可见度、颜色等
	/// @param Percent 填充百分比
	void SetHealthBar(float Percent) const;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar *HealthBar;

	// 生命值小于等于这个数才会显示
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	float BarCanVisiblePercent = 0.8f;

	// 生命值小于等于这个数会改变颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	float BarColorChangePercent = 0.3f;

	// 生命值满时的颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	FLinearColor FullHealthColor = FLinearColor::Black;

	// 生命值低于BarColorChangePercent时的颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	FLinearColor LowHealthColor = FLinearColor::Red;
};
