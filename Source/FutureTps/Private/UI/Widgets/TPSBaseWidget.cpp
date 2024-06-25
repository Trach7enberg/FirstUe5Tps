// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSBaseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UTPSBaseWidget::Show()
{
	if (!IsAnimationPlaying(ShowAnimation))
	{
		PlayAnimation(ShowAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true);
	}
	if (!GetWorld() && !OpenWidgetSound) { return; }
	
	UGameplayStatics::PlaySound2D(GetWorld(), OpenWidgetSound);
	
}
