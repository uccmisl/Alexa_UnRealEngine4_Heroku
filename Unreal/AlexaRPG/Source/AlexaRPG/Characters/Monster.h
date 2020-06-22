// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacter.h"
#include "Monster.generated.h"


class ASpawnerArea;

/**
 *	The base class for every hostile characters non playable characters
 */
UCLASS()
class ALEXARPG_API AMonster : public ARPGCharacter
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	//	Called when the character die
	virtual void Die(ARPGCharacter* killer) override;
public:
	
	//	The spawnner that created this Monster
	ASpawnerArea* spawner;

	//	The money that the monster will have
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int StartingCoins=10;

	//	The experience that the player will gain when he will kill this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int DroppedEXp = 10;
};
