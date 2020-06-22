// Fill out your copyright notice in the Description page of Project Settings.


#include "RegenBuff.h"
#include "AlexaRPG/Characters/RPGCharacter.h"

URegenBuff::URegenBuff()
{
	Duration = 2;
}

void URegenBuff::Apply(ARPGCharacter* target) {
	Super::Apply(target);
	Target->HealthRegen += 10;
}

void URegenBuff::DoEndBuff() {
	Super::DoEndBuff();
	Target->HealthRegen -= 10;
}