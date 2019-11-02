// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "EnhancedPlayerContoller.h"
#include "EnhancedGameMode.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameChoicesWidget.generated.h"

/**
 * 
 */
//#define 

UCLASS()
class READFROMCONFIGFILE_API UGameChoicesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UGameChoicesWidget(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;


	UFUNCTION()
	void OnButtonClickedCallback(int32 ButtonIndex);

	FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero);
		
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* HorizontalBoxContainer = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* CanvasPanelTop = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* CanvasPanelMiddle = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* CanvasPanelBottom = nullptr;
	

	UPROPERTY()
	class ULevelSequencePlayer* LevelSequencePlayer;

	AEnhancedPlayerContoller* PlayerController = nullptr;
	AEnhancedGameMode* EnhancedGameMode = nullptr;
	TMap<int32, FChoicesPerRow1> ChoicesPerLevel;
	TSubclassOf<class UUserWidget> ToolTipWidgetBPClass;
	UUserWidget*  ToolTipWidget = nullptr;
	float PrevMusicValue = -10;
	int32 LevelCounter = 0;
};
