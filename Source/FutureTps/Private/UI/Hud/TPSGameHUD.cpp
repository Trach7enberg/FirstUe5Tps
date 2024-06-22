// FutureTPS Game All Rights Reserved


#include "UI/Hud/TPSGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "GameMode/TPSGameModeBase.h"
#include "CoreTypes/CoreType.h"	

DEFINE_LOG_CATEGORY_STATIC(MyATPSGameHUDLog, All, All);

void ATPSGameHUD::DrawHUD()
{
	Super::DrawHUD();
	// DrawCrossHair();
}


void ATPSGameHUD::BeginPlay()
{
	Super::BeginPlay();


	MatchStateWidgets.Add(ETPSMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayHUDWidgetClass));
	MatchStateWidgets.Add(ETPSMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseHUDWidgetClass));
	MatchStateWidgets.Add(ETPSMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOVerHUDWidgetClass));

	for (auto Widget : MatchStateWidgets)
	{
		const auto GameWidget = Widget.Value;
		if (!GameWidget) { continue; }
		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode) { GameMode->OnMatchStateChanged.AddUObject(this, &ATPSGameHUD::OnMatchStateChanged); }
	}

}

void ATPSGameHUD::DrawCrossHair()
{

	// TInterval T类型的区间 模板
	// 也就是供我们方便的操作 min和max区间的一个模板类,比如说某个点在不在这个区间就可以调用这个模板里的函数
	// 这里我们只是用来存储幕中心点的xy位置
	// const TInterval<float> Center{Canvas->SizeX *.5f, Canvas->SizeY *.5f };

	// Canvas->SizeX 和 Canvas->SizeY 分别获取当前屏幕的宽和高是多少,我们乘以0.5就能得出中心点的位置
	FVector2D Center = FVector2D{Canvas->SizeX * .5f, Canvas->SizeY * .5f};

	// 创建半条线,然后在上下左右分别画出即可形成准星
	const float HalfLineSize = 30.f;
	const float LineThickness = 2.f;
	const FLinearColor LineColor = FLinearColor::Blue;

	// 画水平线断
	DrawLine(Center.X - HalfLineSize, Center.Y, Center.X + HalfLineSize, Center.Y, LineColor, LineThickness);
	// 画垂直线
	DrawLine(Center.X, Center.Y + HalfLineSize, Center.X, Center.Y - HalfLineSize, LineColor, LineThickness);
}

void ATPSGameHUD::OnMatchStateChanged(const ETPSMatchState MatchState)
{
	// 隐藏当前的Widget
	if (CurrentMatchStateWidget) { CurrentMatchStateWidget->SetVisibility(ESlateVisibility::Hidden); }

	// 获取新的Widget
	if (MatchStateWidgets.Contains(MatchState)) { CurrentMatchStateWidget = MatchStateWidgets[MatchState]; }

	// 然后显示新的Widget
	if (CurrentMatchStateWidget) { CurrentMatchStateWidget->SetVisibility(ESlateVisibility::Visible); }

	UE_LOG(MyATPSGameHUDLog, Error, TEXT("GameStateChanged: %s"), *UEnum::GetValueAsString(MatchState));
}
