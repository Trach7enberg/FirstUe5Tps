// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSBaseWidget.h"

void UTPSBaseWidget::Show()
{
	if (!IsAnimationPlaying(ShowAnimation))
	{
		PlayAnimation(ShowAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true);
	}
}
