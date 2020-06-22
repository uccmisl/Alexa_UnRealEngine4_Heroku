// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff.h"
#include "AttackBuff.generated.h"

/**
 * A Buff that increase the attack damage of a character
 */
UCLASS()
class ALEXARPG_API UAttackBuff : public UBuff
{
	GENERATED_BODY()
	
public:


	//	Aplly the effects of the buff and start the Duration Timer
	virtual void Apply(ARPGCharacter* target) override;

	//	delete buff effects
	virtual void DoEndBuff() override;
};
