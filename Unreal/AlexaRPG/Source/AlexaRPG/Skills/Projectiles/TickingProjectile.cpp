// Fill out your copyright notice in the Description page of Project Settings.


#include "TickingProjectile.h"


void ATickingProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (hitBox != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("testset"));
		hitBox->OnComponentEndOverlap.AddDynamic(this, &ATickingProjectile::OnOverlapEnd);
		hitBox->SetCollisionProfileName("OverlapAll");
	}
	remainingTime = duration;
	GetWorld()->GetTimerManager().SetTimer(TickTimer, this, &ATickingProjectile::tick, tickFrequency, true);
}

void ATickingProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor == Caster) return;
	ARPGCharacter* target = Cast<ARPGCharacter>(OtherActor);
	if (target != nullptr) {
		targets.Add(target);
	}
}

void  ATickingProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor == Caster) return;
	ARPGCharacter* target = Cast<ARPGCharacter>(OtherActor);
	if (target != nullptr) {
		targets.Remove(target);
	}
}

void ATickingProjectile::tick() {
	if (remainingTime <= 0) {
		Destroy();
		return;
	}
	remainingTime -= tickFrequency;
	for (auto target : targets) {
		if (target != nullptr) {
			applyEffects(target);
		}
	}
}