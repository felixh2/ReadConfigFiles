// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "GameStateButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, int32, Index);

/**
 * 
 */
UCLASS()
class READFROMCONFIGFILE_API UGameStateButton : public UButton
{
	GENERATED_BODY()

public:

	UGameStateButton();
	
	UFUNCTION()
	void OnClick();

	UPROPERTY()
	FOnButtonClicked OnButtonClicked;

	int32 ButtonIndex;

};
