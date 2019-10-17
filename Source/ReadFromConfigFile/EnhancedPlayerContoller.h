// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedPlayerContoller.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChosen, int32, Index);

USTRUCT(BlueprintType)
struct FChoicesPerRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "ChoicesStruct")
	float MusicTime;

	UPROPERTY(BlueprintReadOnly, Category = "ChoicesStruct")
	FVector PlayerPosition;

	UPROPERTY(BlueprintReadOnly, Category = "ChoicesStruct")
	TMap<FString, bool> ChoicesPerRow;
};


UENUM(BlueprintType)
enum class ELevelsEnum : uint8
{
	House,
	Bar,
	Alley		
};



UCLASS(Config = Game)
class READFROMCONFIGFILE_API AEnhancedPlayerContoller : public APlayerController
{
	GENERATED_BODY()
public:
	
	AEnhancedPlayerContoller();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	TMap<int32, FChoicesPerRow> ReadFromConfigFile();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game State Event")
	FOnGameStateChosen OnGameStateChosen;

public:

	TMap<int32, FChoicesPerRow> GetChoicesPerLevel();

protected:

	//UPROPERTY(BlueprintReadOnly, Category = "Game State")
	TMap<int32, FChoicesPerRow> ChoicesPerLevel;


	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	TArray<float> MusicTimeArray;

private:
	void ParseQueryString(FString QueryString);

	int32 RowIndex = 1;
	float MusicTimePerRow;
	TMap<FString, bool> ChoicesPerRow;
	
	
	
};



