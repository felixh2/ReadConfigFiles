// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedToolTip.generated.h"

/**
 * 
 */
UCLASS()
class READFROMCONFIGFILE_API UEnhancedToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetEnhancedToolTipText(FText Text);

public:
		UPROPERTY(meta = (BindWidget))
		class UTextBlock* ToolTipTextBlock = nullptr;
};
