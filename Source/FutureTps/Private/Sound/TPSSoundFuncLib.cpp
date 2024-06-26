// FutureTPS Game All Rights Reserved


#include "Sound/TPSSoundFuncLib.h"

#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSSoundFuncLibLog, All, All);

void UTPSSoundFuncLib::SetSoundClassVolume(USoundClass *SoundClass, float volume)
{
	if (!SoundClass) { return; }
	SoundClass->Properties.Volume = FMath::Clamp(volume, 0.0f, 1.0f);
	
	UE_LOG(MyUTPSSoundFuncLibLog, Error, TEXT("%f"), SoundClass->Properties.Volume);
}


void UTPSSoundFuncLib::ToggleSoundClassVolume(USoundClass *SoundClass)
{
	if (!SoundClass) { return; }
	SetSoundClassVolume(SoundClass, (SoundClass->Properties.Volume > 0.0f) ? 0.0f : 1.0f);
}
