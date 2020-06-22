// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "AlexaRPG/Characters/RPGCharacter.h"

USkill::USkill() {
	Name = "ASkill";
}

USkill::USkill(FString name) : Name(name), Caster(nullptr){
}

void USkill::Cast() {
	if (canBeCast) {
		if (Caster->ConsumeMana(ManaCost)) {
			canBeCast = false;
			RemainingTime = Cooldown;
			Caster->GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &USkill::UpdateCooldown, UpdateRate, true, 0.1);
			DoCast();
		}
	}
}

void USkill::UpdateCooldown() {
	RemainingTime -= UpdateRate;
	if (RemainingTime <= 0) {
		RemainingTime = 0;
		canBeCast = true;
		Caster->GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
	}
}

void USkill::DoCast() {

}