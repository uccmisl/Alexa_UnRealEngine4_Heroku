// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "Skill.generated.h"

class ARPGCharacter;


/*
* The base class for every skills in the game
*/
UCLASS()
class ALEXARPG_API USkill : public UObject
{
	GENERATED_BODY()
	

public:
	USkill();
	USkill(FString name);


	//	The skill's Name
	UPROPERTY(BlueprintReadOnly)
		FString Name;

	//	The amount of mana needed to use the skill
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ManaCost = 10;

	//	The time you will need to wait after casting this skill to use it again
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Cooldown=10;

	//	The rate at wich the Cooldown Update method will be executed
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float UpdateRate = 1;

	//	The remaining time before you can use the skill
	UPROPERTY(BlueprintReadOnly)
		float RemainingTime =0;

	//	Say if the skill  can be cast
	UPROPERTY(BlueprintReadOnly)
		bool canBeCast = true;

	//	The timer used to update Remaining time
	FTimerHandle CooldownTimer;

	//	The character who cast the skill
	ARPGCharacter* Caster;

	//	Cast the skill if possible and start the Cooldown Timer
	UFUNCTION(BlueprintCallable)
		void Cast();

	// used to implement the skill's effects
	virtual void DoCast();

	//	Called by the cooldown timer, update the remaining time
	void UpdateCooldown();
};
