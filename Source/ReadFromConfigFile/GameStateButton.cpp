// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateButton.h"


UGameStateButton::UGameStateButton()
{
	OnClicked.AddDynamic(this, &UGameStateButton::OnClick);
	
}

void UGameStateButton::OnClick()
{
	OnButtonClicked.Broadcast(ButtonIndex);
}
