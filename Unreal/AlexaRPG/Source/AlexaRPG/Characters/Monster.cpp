// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "AlexaRPG/SpawnerArea.h"
#include "AlexaRPG/Items/LootingBag.h"
#include "Math/UnrealMathUtility.h"
#include "AlexaRPG/Items/Weapon.h"
#include "PlayableCharacter.h"

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	inventory->AddCoins(StartingCoins);
}

void AMonster::Die(ARPGCharacter* killer)
{
	Super::Die(killer);
	APlayableCharacter* player = Cast<APlayableCharacter>(killer);
	if (player != nullptr) {
		player->GainExp(DroppedEXp);
	}
	ALootingBag* loots = GetWorld()->SpawnActor<ALootingBag>(GetActorLocation(), FRotator(0, 0, 0), FActorSpawnParameters());
	loots->Coins += FMath::RandRange(0.5f, 1.0f)*this->inventory->Coins;
	if (Weapon != nullptr) {
		Weapon->ChangeVisibility(false);
		if (FMath::RandBool()) {
			loots->AddItem(Weapon);
		}
	}
	for (auto& i : this->inventory->Items) {
		if (FMath::RandBool()) {
			if (!loots->AddItem(i.Value)) {
				break;
			}
		}
	}
	

	if (spawner != nullptr) {
		spawner->EntitiesCount--;
	}
	this->Destroy();
}