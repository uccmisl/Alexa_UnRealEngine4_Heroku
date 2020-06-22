// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlexaRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API AAlexaRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		int sessionId = 0;
	
};
