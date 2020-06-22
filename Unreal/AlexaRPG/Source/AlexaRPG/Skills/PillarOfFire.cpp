// Fill out your copyright notice in the Description page of Project Settings.


#include "PillarOfFire.h"
#include "UObject/ConstructorHelpers.h"

UPillarOfFire::UPillarOfFire() : USkill("Pillar of fire") {

	UE_LOG(LogTemp, Warning, TEXT("Construct"));

	Cooldown = 3;
	
	static ConstructorHelpers::FClassFinder<ATickingProjectile> ClassFinder(TEXT("/Game/AlexaRPG/Blueprints/Projectiles/FirePillar"));
	if (ClassFinder.Succeeded()) {

		UE_LOG(LogTemp, Warning, TEXT("Found Pillar"));
		ProjectileClass = ClassFinder.Class;
	}
}

void UPillarOfFire::DoCast() {
	if (ProjectileClass != nullptr) {

		UE_LOG(LogTemp, Warning, TEXT("SpawnPillar"));
		ATickingProjectile* SpawnedActor = Caster->GetWorld()->SpawnActor<ATickingProjectile>(ProjectileClass, Caster->GetActorLocation() + Caster->GetActorRotation().RotateVector(FVector(500,0,0)), FRotator(0, 0, 0), FActorSpawnParameters());

		if (SpawnedActor != nullptr) {
			SpawnedActor->Caster = Caster;
		}
	}
}