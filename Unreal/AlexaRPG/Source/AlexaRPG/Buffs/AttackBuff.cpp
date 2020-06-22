// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBuff.h"
#include "AlexaRPG/Characters/RPGCharacter.h"


void UAttackBuff::Apply(ARPGCharacter* target) {
	Super::Apply(target);
	Target->baseDamage += 1000;
}

void UAttackBuff::DoEndBuff() {
	Super::DoEndBuff();
	Target->baseDamage -= 1000;
}