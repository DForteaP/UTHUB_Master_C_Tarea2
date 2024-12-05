// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tarea2GameMode.h"
#include "Tarea2Character.h"
#include "UObject/ConstructorHelpers.h"

ATarea2GameMode::ATarea2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
