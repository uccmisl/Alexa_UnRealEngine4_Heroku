// Fill out your copyright notice in the Description page of Project Settings.


#include "RegenPotion.h"
#include "AlexaRPG/Characters/RPGCharacter.h"
#include "AlexaRPG/Buffs/RegenBuff.h"

void ARegenPotion::Use(ARPGCharacter* itemOwner) {
	itemOwner->AddBuff(NewObject<UBuff>(GetTransientPackage(), URegenBuff::StaticClass()));
	Super::Use(itemOwner);
}