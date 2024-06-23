// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoreTypes/CoreType.h"
#include "TPSLevelSelectItemWidget.generated.h"

class UTPSGameInstance;
class UImage;
class UTextBlock;
class UButton;
/**
 * 关卡选择小部件
 */
UCLASS()
class FUTURETPS_API UTPSLevelSelectItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLevelSelectedSignature OnLevelSelected;

	FLevelData GetLevelData() const { return LevelData; }
	void SetLevelData(const FLevelData &Data);

	void SetSelected(bool IsSelected);

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	UButton *LevelSelectButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock *LevelNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UImage *LevelImageFrame = nullptr;

	UPROPERTY(meta=(BindWidget))
	UImage *LevelImage = nullptr;

private:
	// 当前选择关卡的数据,在menu的widget中被初始化
	FLevelData LevelData {};

	UFUNCTION()
	void OnSelectedLevelItem();


};
