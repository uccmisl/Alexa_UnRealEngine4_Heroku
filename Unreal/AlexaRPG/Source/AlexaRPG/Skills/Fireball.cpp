// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"
#include "AlexaRPG/Characters/RPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

UFireball::UFireball() : USkill("Fireball") {

	UE_LOG(LogTemp, Warning, TEXT("Construct"));

	Cooldown = 3;
	static ConstructorHelpers::FClassFinder<AProjectile> ClassFinder(TEXT("/Game/AlexaRPG/Blueprints/Projectiles/Fireball"));
	if (ClassFinder.Succeeded()) {
		ProjectileClass = ClassFinder.Class;
	}
}

void UFireball::DoCast() {
	if (ProjectileClass != nullptr) {
		AProjectile* SpawnedActor = Caster->GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Caster->GetPawnViewLocation() + FVector(0, 0, 40), FRotator(0, 0, 0), FActorSpawnParameters());

		if (SpawnedActor != nullptr) {
			SpawnedActor->Caster = Caster;
			SpawnedActor->SetInitialDirection(Caster->GetViewRotation().RotateVector(FVector(1, 0, 0)));
		}
	}
}