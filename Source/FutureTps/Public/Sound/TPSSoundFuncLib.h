// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPSSoundFuncLib.generated.h"

class USoundClass;

/**
 * 管理声音的功能类
 */
UCLASS()
class FUTURETPS_API UTPSSoundFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void SetSoundClassVolume(USoundClass *SoundClass, float volume);

	UFUNCTION(BlueprintCallable)
	static void ToggleSoundClassVolume(USoundClass *SoundClass);

	
};
