// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSLevelSelectItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSLevelSelectItemWidgetLog,All,All);

void UTPSLevelSelectItemWidget::NativeOnInitialized()
{

	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &UTPSLevelSelectItemWidget::OnSelectedLevelItem);
	}
	Super::NativeOnInitialized();
}

void UTPSLevelSelectItemWidget::SetLevelData(const FLevelData &Data)
{
	LevelData = Data;
	if (LevelNameTextBlock) { LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName)); }
	if (LevelImage) { LevelImage->SetBrushFromTexture(Data.LevelThumbnail); }
}

void UTPSLevelSelectItemWidget::SetSelected(bool IsSelected)
{
	if (LevelImageFrame)
	{
		LevelImageFrame->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UTPSLevelSelectItemWidget::OnSelectedLevelItem()
{
	
	OnLevelSelected.Broadcast(LevelData);
}
