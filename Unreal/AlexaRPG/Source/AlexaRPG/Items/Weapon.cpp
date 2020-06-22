// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "UObject/ConstructorHelpers.h"
#include "AlexaRPG/Characters/RPGCharacter.h"

// Sets default values
AWeapon::AWeapon() : AItem()
{

	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));

	if (VisualAsset.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(VisualAsset.Object);
		SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}


	HitBox->SetRelativeScale3D(FVector(0.1f, 0.6f, 2.1f));
	HitBox->SetRelativeLocation(FVector(0, 0, 48));




	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Use(ARPGCharacter* character) {
	UseImplementation(character);
	if(WeaponOwner == nullptr) {
		character->EquipWeapon(this);
	}
	else {
		WeaponOwner->UnequipWeapon();
	}
}

bool AWeapon::canDealDomageMultipleTimes() {

	UE_LOG(LogTemp, Warning, TEXT("can domage"));
	return false;
}