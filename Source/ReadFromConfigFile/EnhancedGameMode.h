// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnhancedGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChosen1, int32, Index);

USTRUCT(BlueprintType)
struct FChoicesPerRow1
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "ChoicesStruct")
		float MusicTime;

	UPROPERTY(BlueprintReadOnly, Category = "ChoicesStruct")
		FVector PlayerPosition;

	UPROPERTY(BlueprintReadOnly, Category = "ChoicesStruct")
		TMap<FString, bool> ChoicesPerRow;
};



UCLASS(Config = Game)
class READFROMCONFIGFILE_API AEnhancedGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
		TMap<int32, FChoicesPerRow1> ReadFromConfigFile1();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game State Event")
		FOnGameStateChosen1 OnGameStateChosen;

public:

	TMap<int32, FChoicesPerRow1> GetChoicesPerLevel1();

protected:
	TMap<int32, FChoicesPerRow1> ChoicesPerLevel;


	UPROPERTY(BlueprintReadOnly, Category = "Game State")
		TArray<float> MusicTimeArray;

private:
	void ParseQueryString1(FString QueryString);

	int32 RowIndex = 1;
	float MusicTimePerRow;
	TMap<FString, bool> ChoicesPerRow;
};
