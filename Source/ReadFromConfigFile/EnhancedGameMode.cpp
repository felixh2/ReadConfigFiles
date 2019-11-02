// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedGameMode.h"

#include "EnhancedPlayerContoller.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "TimerManager.h"


void AEnhancedGameMode::BeginPlay()
{
	Super::BeginPlay();

}

TMap<int32, FChoicesPerRow1> AEnhancedGameMode::ReadFromConfigFile1()
{

	if (!GConfig) return TMap<int32, FChoicesPerRow1>();

	FString QueryString;
	int32 Key = 1;
	int32 EnumLevelIndex = 0;

	// First Option - Per Level
	UWorld* World = GetWorld();
	FString LevelName = World->GetName();

	// Second Option
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ELevelsEnum"), true);
	//if (!EnumPtr) return TMap<int32, FChoicesPerRow1>();


	//while (EnumPtr->GetNameByValue(EnumLevelIndex) != NAME_None) {

	//FString LevelName = EnumPtr->GetNameStringByIndex(EnumLevelIndex);
	//Key = 1;

	// TODO: Adjust level name to our project
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LevelName);
	while (GConfig->GetString((TEXT("%s"), *LevelName), (TEXT("%s"), *FString::FromInt(Key)), QueryString, GGameIni)) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *QueryString);
		ParseQueryString1(QueryString);

		Key++;
	}

	//	EnumLevelIndex++;
	//}


	//ClientMessage(QueryString);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *QueryString);

	/*
		GEditorPerProjectIni;
		GInputIni;
		GEditorIni;
		GEditorPerProjectIni;
		GEngineIni
	*/
	return ChoicesPerLevel;
}

// Called For each line in the Configuration file - Heavy
// TODO - remove from player controller class
void AEnhancedGameMode::ParseQueryString1(FString QueryString)
{
	int32 ArrayIndex = 1;
	TArray<FString> MajorParsedQueryString;
	QueryString.ParseIntoArray(MajorParsedQueryString, TEXT(","), true);
	MusicTimePerRow = FCString::Atof(*MajorParsedQueryString[0]);

	ChoicesPerRow.Reset();

	FChoicesPerRow1 ChoicesPerRowStruct;

	// parse the major string
	while (ArrayIndex < MajorParsedQueryString.Num())
	{

		// if the below statement true - we have a position state request
		if (MajorParsedQueryString[ArrayIndex].Contains("Position")) {
			TArray<FString> MajorPositionParsedQueryString;
			TArray<FString> MinorPositionParsedQueryString;
			MajorParsedQueryString[MajorParsedQueryString.Num() - 1].ParseIntoArray(MajorPositionParsedQueryString, TEXT("="), true);

			MajorPositionParsedQueryString[1].RemoveAt(0, 1, true);
			MajorPositionParsedQueryString[1].RemoveAt(MajorPositionParsedQueryString[1].Len() - 1, 1, true);
			MajorPositionParsedQueryString[1].ParseIntoArray(MinorPositionParsedQueryString, TEXT(";"), true);

			float PositionX = FCString::Atof(*MinorPositionParsedQueryString[0]);
			float PositionY = FCString::Atof(*MinorPositionParsedQueryString[1]);
			float PositionZ = FCString::Atof(*MinorPositionParsedQueryString[2]);
			FVector Position = FVector(PositionX, PositionY, PositionZ);

			ChoicesPerRowStruct.PlayerPosition = Position;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Position.ToString());
		}
		else if (MajorParsedQueryString[ArrayIndex].Contains("Description")) {
			ChoicesPerRowStruct.Description = MajorParsedQueryString[ArrayIndex];
		}
		else {
			TArray<FString> MinorParsedQueryString;
			MajorParsedQueryString[ArrayIndex].ParseIntoArray(MinorParsedQueryString, TEXT("="), true);
			FString BooleanName = MinorParsedQueryString[0];
			bool BooleanState = MinorParsedQueryString[1].ToBool();

			ChoicesPerRow.Add(BooleanName, BooleanState);

			ChoicesPerRowStruct.ChoicesPerRow.Add(BooleanName, BooleanState);
		}
		ArrayIndex++;
	}

	ChoicesPerRowStruct.MusicTime = MusicTimePerRow;
	ChoicesPerLevel.Add(RowIndex, ChoicesPerRowStruct);

	MusicTimeArray.Add(MusicTimePerRow);
	RowIndex++;

	UE_LOG(LogTemp, Warning, TEXT("%f"), MusicTimePerRow);


}

TMap<int32, FChoicesPerRow1> AEnhancedGameMode::GetChoicesPerLevel1()
{
	return ChoicesPerLevel;
}