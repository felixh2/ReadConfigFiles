// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedToolTip.h"
#include "Components/TextBlock.h"


void UEnhancedToolTip::SetEnhancedToolTipText(FText Text)
{
	if (!ToolTipTextBlock) { return; }
	
	ToolTipTextBlock->SetText(Text);
}