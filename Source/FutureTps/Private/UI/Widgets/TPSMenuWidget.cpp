// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSMenuWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "GameInstances/TPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/TPSLevelSelectItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSMenuWidgetLog, All, All);

void UTPSMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartGameButton) { StartGameButton->OnClicked.AddDynamic(this, &UTPSMenuWidget::OnStartGame); }
	InitLevelItems();
}

void UTPSMenuWidget::OnStartGame()
{
	if (!GetWorld()) { return; }


	const auto TPSGameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!TPSGameInstance) { return; }

	const auto StartLevel = TPSGameInstance->GetStartLevel();


	UGameplayStatics::OpenLevel(this, StartLevel.LevelName);

}

void UTPSMenuWidget::InitLevelItems()
{
	const auto GameInstance = GetTPSGameInstance();
	if (!GameInstance) { return; }

	checkf(GameInstance->GetLevelsData().Num() > 0, TEXT("Levels data must be added in GameInstance"));

	if (!LevelSelectItemWidgetClass || !LevelItemArea) { return; }
	LevelItemArea->ClearChildren();

	for (auto LevelData : GameInstance->GetLevelsData())
	{
		auto LevelItemWidget = CreateWidget<UTPSLevelSelectItemWidget>(GetWorld(), LevelSelectItemWidgetClass);
		if (!LevelItemWidget) { continue; }

		if (IsDuplicateLevel(LevelData))
		{
			UE_LOG(MyUTPSMenuWidgetLog, Warning, TEXT("Level [%s] is duplicate"), *LevelData.LevelName.ToString());
			continue;
		}


		LevelItemWidget->SetLevelData(LevelData);

		LevelItemWidget->OnLevelSelected.AddUObject(this, &UTPSMenuWidget::OnSelectedLevelItem);

		LevelItemArea->AddChild(LevelItemWidget);


		LevelItemWidgets.Add(LevelItemWidget->GetLevelData().LevelName, LevelItemWidget);
	}

	// 初始化时默认选择第一个关卡作为默认聚显示关卡
	if (GameInstance->GetStartLevel().LevelName.IsNone()) { OnSelectedLevelItem(GameInstance->GetLevelsData()[0]); }
	else
	{
		// 当默认的开始关卡不为None的话,比如我们从暂停界面返回主菜单,那么默认就会聚焦显示上一次选择的关卡
		OnSelectedLevelItem(GameInstance->GetStartLevel());
	}
}

UTPSGameInstance *UTPSMenuWidget::GetTPSGameInstance() const
{
	if (!GetWorld()) { return nullptr; }

	return GetWorld()->GetGameInstance<UTPSGameInstance>();
}


void UTPSMenuWidget::OnSelectedLevelItem(const FLevelData &Data)
{

	const auto GameInstance = GetTPSGameInstance();
	if (!GameInstance) { return; }

	GameInstance->SetStartLevel(Data);
	if (LevelItemWidgets.Contains(Data.LevelName))
	{
		LevelItemWidgets[Data.LevelName]->SetSelected(true);

		if (!LastSelected.IsNone() && LastSelected != Data.LevelName) { LevelItemWidgets[LastSelected]->SetSelected(false); }

		LastSelected = (Data.LevelName);
	}
}

bool UTPSMenuWidget::IsDuplicateLevel(const FLevelData &Data) const
{
	for (const auto Element : LevelItemWidgets)
	{
		if (Element.Value->GetLevelData().LevelName == Data.LevelName) { return true; }
	}
	return false;
}
