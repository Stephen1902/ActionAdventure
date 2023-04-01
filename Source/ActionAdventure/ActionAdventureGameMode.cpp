// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionAdventureGameMode.h"
#include "ActionAdventureCharacter.h"
#include "UObject/ConstructorHelpers.h"

AActionAdventureGameMode::AActionAdventureGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MetaHumans/JayJones2/BP_JayJones3"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
