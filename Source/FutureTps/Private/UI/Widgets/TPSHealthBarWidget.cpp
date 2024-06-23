// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSHealthBarWidget.h"

#include "Components/ProgressBar.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSHealthBarWidgetLog,All,All);

void UTPSHealthBarWidget::SetHealthBar(const float Percent) const
{
	if (!HealthBar) { return; }
	
	const auto BarVisibility = (!FMath::IsNearlyZero(Percent) && Percent <= BarCanVisiblePercent)
		? ESlateVisibility::Visible
		: ESlateVisibility::Hidden;

	HealthBar->SetVisibility(BarVisibility);

	const auto Color = (Percent <= BarColorChangePercent)
		? LowHealthColor
		: FullHealthColor;
	
	HealthBar->SetFillColorAndOpacity(Color);
	HealthBar->SetPercent(Percent);
}
