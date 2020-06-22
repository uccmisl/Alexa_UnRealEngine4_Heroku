// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buff.generated.h"

class ARPGCharacter;

/**
 *	The base class for every buffs, they are used to modify temporarily the stats of a character
 */
UCLASS()
class ALEXARPG_API UBuff : public UObject
{
	GENERATED_BODY()
	
public:

	//	Called when the character already have this type of buff, reset the remaining of the buff in place
	void Update(UBuff* buff);

	//	Aplly the effects of the buff and start the Duration Timer
	virtual void Apply(ARPGCharacter* target);

	//	The character who will receive the buff
	ARPGCharacter* Target;

	//	Called to update the duration an delete buff effects (by calling DoEndBuff) and the buff itself when duration is reached
	void EndBuff();

	//	used to implement how to delete buff effects
	virtual void DoEndBuff();


	//	The timer used to update Remaining time
	FTimerHandle DurationTimer;

	//	The duration of the buff
	float Duration = 10;

	//	The time before the buff will get delete
	float RemainingTime = 0;

	//	The rate at wich the EndBuff method will be executed
	float UpdateTime = 1;
};
