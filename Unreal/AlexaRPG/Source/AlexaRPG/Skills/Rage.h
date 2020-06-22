// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Rage.generated.h"


/*
*	An ability that increase the base damage of a character for a short period of time
*/
UCLASS()
class ALEXARPG_API URage : public USkill
{
	GENERATED_BODY()
	
public:
	URage();

	//	The class of the Particle systems (set in the constructor)
	UParticleSystem* ParticleSystem;

	//	Th particul component
	UParticleSystemComponent* ParticleSystemComp;

	// implements the skill's effects: spawn paricles and give an attack buff
	virtual void DoCast() override;

	//	The timer used to stop the particle animation
	FTimerHandle AnimTimer;

	//	Stop the particle Animation
	void StopAnim();
};
