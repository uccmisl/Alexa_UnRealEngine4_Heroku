// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff.h"
#include "RegenBuff.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API URegenBuff : public UBuff
{
	GENERATED_BODY()
	

public:
	URegenBuff();
	//	Aplly the effects of the buff and start the Duration Timer
	virtual void Apply(ARPGCharacter* target) override;

	//	delete buff effects
	virtual void DoEndBuff() override;
};
