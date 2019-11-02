// Fill out your copyright notice in the Description page of Project Settings.


#include "GameChoicesWidget.h"

#include "Engine.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/EditableTextBox.h"
#include "GameStateButton.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "EnhancedGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedToolTip.h"
#include "Layout/Geometry.h"

//#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/MovieScene/Public/MovieSceneSequencePlayer.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"

//https://answers.unrealengine.com/questions/446202/populate-umg-widget-dynamically-from-c.html

#pragma optimize("", off)
UGameChoicesWidget::UGameChoicesWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ToolTipBPClass(TEXT("/Game/BPW_ToolTip"));
	if (ToolTipBPClass.Class != NULL)
	{
		ToolTipWidgetBPClass = ToolTipBPClass.Class;
	}

}


bool UGameChoicesWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!ensure(Success)) { return false; }
	
	if (!HorizontalBoxContainer) { return false; }

	UWorld* World = GetWorld();
	if (!ensure(World)) {return false;}

	PlayerController = Cast<AEnhancedPlayerContoller>(World->GetFirstPlayerController());
	if (!ensure(PlayerController)) { return false; }

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	int32 OutViewPortSizeX, OutViewPortSizeY;
	PlayerController->GetViewportSize(OutViewPortSizeX, OutViewPortSizeY);
	
	//ChoicesPerLevel = PlayerController->GetChoicesPerLevel();

	EnhancedGameMode = Cast<AEnhancedGameMode>(GetWorld()->GetAuthGameMode());
	if (!ensure(EnhancedGameMode)) { return false; }
	ChoicesPerLevel = EnhancedGameMode->GetChoicesPerLevel1();

	for (const TPair<int32, FChoicesPerRow1>& Pair : ChoicesPerLevel)
	{
		FString Key = FString::FromInt(Pair.Key);
		FChoicesPerRow1 Value = Pair.Value;
		
		UGameStateButton* GameStateButton = NewObject<UGameStateButton>();
		GameStateButton->ButtonIndex = Pair.Key;	
		UGameStateButton* GameStateButton1 = NewObject<UGameStateButton>();
		GameStateButton1->ButtonIndex = Pair.Key;

		ToolTipWidget = CreateWidget<UUserWidget>(this, ToolTipWidgetBPClass);
		
		FString ToolTipMusicString = GetFloatAsStringWithPrecision(Value.MusicTime, 3, true);
		FString ToolTipText; // = FString::SanitizeFloat(Value.MusicTime);
		ToolTipText = ToolTipMusicString + "\n";
		ToolTipText += Value.Description + "\n";
		TArray<FString> GameStateNames;
		Value.ChoicesPerRow.GetKeys(GameStateNames);
		for (FString GameStateName : GameStateNames) {
			bool GameStateCondition = Value.ChoicesPerRow.FindRef(GameStateName);
			FString GameStateConditionString = GameStateCondition ? "True" : "False";
			ToolTipText = ToolTipText + GameStateName + " = " + GameStateConditionString + "\n";
		}
		
		Cast<UEnhancedToolTip>(ToolTipWidget)->SetEnhancedToolTipText(FText::FromString(ToolTipText));
		GameStateButton1->SetToolTip(Cast<UUserWidget>(ToolTipWidget));


		FWidgetTransform WidgetTransform;
		WidgetTransform.Scale = FVector2D(1.5, 1.0);
		GameStateButton->SetRenderTransform(WidgetTransform);
		//GameStateButton1->SetRenderTransform(WidgetTransform);

		GameStateButton1->OnButtonClicked.AddDynamic(this, &UGameChoicesWidget::OnButtonClickedCallback);		
		UTextBlock* TextBox = NewObject<UTextBlock>();

		TextBox->SetText(FText::FromString(Key));
		GameStateButton1->AddChild(TextBox);
		//GameStateButton1->AddChild(TextBox);

		/*
		UHorizontalBoxSlot* Slot = HorizontalBoxContainer->AddChildToHorizontalBox(GameStateButton);

		FSlateChildSize SlateChildSize;
		SlateChildSize.Value = 1.0f;
		SlateChildSize.SizeRule = ESlateSizeRule::Fill;
		Slot->SetSize(FSlateChildSize(SlateChildSize));
		Slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		Slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		*/

		if (Value.MusicTime - PrevMusicValue > 8) {
			UCanvasPanelSlot* Slot1 = CanvasPanelTop->AddChildToCanvas(GameStateButton1);
			Slot1->SetSize(FVector2D(40.0, 40.0));
			Slot1->SetPosition(FVector2D((Value.MusicTime / 360.0)*OutViewPortSizeX * 3, 2.0));
		}
		else // Populate second level 
		{
			if (LevelCounter == 0) {
				UCanvasPanelSlot* Slot2 = CanvasPanelMiddle->AddChildToCanvas(GameStateButton1);
				Slot2->SetSize(FVector2D(40.0, 40.0));
				Slot2->SetPosition(FVector2D((Value.MusicTime / 360.0)*OutViewPortSizeX * 3, 2.0));

				LevelCounter++;
			}
			else {
				UCanvasPanelSlot* Slot3 = CanvasPanelBottom->AddChildToCanvas(GameStateButton1);
				Slot3->SetSize(FVector2D(40.0, 40.0));
				Slot3->SetPosition(FVector2D((Value.MusicTime / 360.0)*OutViewPortSizeX * 3, 2.0));
				LevelCounter = 0;
			}
		}

		PrevMusicValue = Value.MusicTime;
	}
	
	return true;
}




void UGameChoicesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	int32 f = 10;
}
void UGameChoicesWidget::NativePreConstruct()
{

	Super::NativePreConstruct();
	
}


void UGameChoicesWidget::OnButtonClickedCallback(int32 ButtonIndex)
{
	//PlayerController->ClientMessage(FString::FromInt(ButtonIndex));

	

	 // Get a list of ULevelSequence
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	TArray<FAssetData> LevelSequenceAssetList;
	AssetRegistry.GetAssetsByClass(ULevelSequence::StaticClass()->GetFName(), LevelSequenceAssetList);
	

	ULevelSequence* LevelSequence = Cast<ULevelSequence>(LevelSequenceAssetList[0].GetAsset());
	FStringAssetReference SequenceName("/Game/LevelSequence2.LevelSequence2");
	ULevelSequence* Asset = Cast<ULevelSequence>(SequenceName.TryLoad());

	//LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
	
	if (Cast<UMovieSceneSequencePlayer>(LevelSequencePlayer)->IsPlaying()) {
		int32 a = 10;
		Cast<UMovieSceneSequencePlayer>(LevelSequencePlayer)->Pause();
	}

	//PlayerController->ClientMessage(FString::FromInt(LevelSequencePlayer->GetDuration()));

	EnhancedGameMode->OnGameStateChosen.Broadcast(ButtonIndex);
}


  FString UGameChoicesWidget::GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero = true)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
	{
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat).ToString();
}
#pragma optimize("", on)