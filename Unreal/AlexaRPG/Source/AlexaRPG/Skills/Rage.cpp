// Fill out your copyright notice in the Description page of Project Settings.


#include "Rage.h"
#include "AlexaRPG/Characters/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "AlexaRPG/Buffs/AttackBuff.h"

URage::URage() : USkill("Rage")
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(TEXT("/Game/InfinityBladeEffects/Effects/FX_Skill_Aura/P_AuraCircle_Default_Base_01.P_AuraCircle_Default_Base_01"));
	if (ParticleFinder.Succeeded()) {
		ParticleSystem = ParticleFinder.Object;
	}
	Cooldown = 20;
}

void URage::DoCast() {
	ParticleSystemComp = UGameplayStatics::SpawnEmitterAttached(ParticleSystem, (USceneComponent*) Caster->GetMesh());
	Caster->GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &URage::StopAnim, 10, false);
	Caster->AddBuff(NewObject<UBuff>(GetTransientPackage(), UAttackBuff::StaticClass()));
}

void URage::StopAnim() {
	ParticleSystemComp->Complete();
}