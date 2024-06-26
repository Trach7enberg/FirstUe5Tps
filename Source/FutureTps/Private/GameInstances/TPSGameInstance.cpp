// FutureTPS Game All Rights Reserved


#include "GameInstances/TPSGameInstance.h"

#include "Sound/TPSSoundFuncLib.h"

void UTPSGameInstance::ToggleVolume() const
{
	UTPSSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
