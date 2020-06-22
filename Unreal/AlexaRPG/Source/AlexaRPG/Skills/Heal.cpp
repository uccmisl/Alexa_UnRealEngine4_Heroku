// Fill out your copyright notice in the Description page of Project Settings.


#include "Heal.h"
#include "AlexaRPG/Characters/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"

UHeal::UHeal() : USkill("Heal")
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Health_Pickup_01.P_Health_Pickup_01"));
	if (ParticleFinder.Succeeded()) {
		ParticleSystem = ParticleFinder.Object;
	}
}


void UHeal::DoCast() {
	UGameplayStatics::SpawnEmitterAtLocation(Caster->GetWorld(), ParticleSystem, Caster->GetActorLocation());
	Caster->Heal(BaseHealAmount);
}