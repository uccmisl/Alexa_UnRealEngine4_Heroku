// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Heal.generated.h"

/**
 *	An ability that heal a character
 */
UCLASS()
class ALEXARPG_API UHeal : public USkill
{
	GENERATED_BODY()
	
public:
	UHeal();

	//	The class of the Particle systems (set in the constructor)
	UParticleSystem* ParticleSystem;

	// implements the skill's effects: retore some Health
	virtual void DoCast() override;

	//	The amount of health restored
	int BaseHealAmount = 20;
};
