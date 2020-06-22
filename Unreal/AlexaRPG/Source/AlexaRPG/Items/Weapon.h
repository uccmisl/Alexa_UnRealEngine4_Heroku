// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AlexaRPG/DamageSource.h"
#include "Weapon.generated.h"

class ARPGCharacter;

/**
 *	The base class for every weapons
 */
UCLASS()
class ALEXARPG_API AWeapon : public AItem, public IDamageSource
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//	The Character that equiped this weapon
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ARPGCharacter* WeaponOwner;

	//	The base damage of the weapon
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float BaseDamage;

	//	Say if damage can be dealt by this weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanDealDamage = false;

	/**
	*	Called when the item is used from the inventory (try equip the weapon to the character)
	*	@param itemOwner - The character which is using this item
	*/
	virtual void Use(ARPGCharacter* character) override;

	virtual bool canDealDomageMultipleTimes() override;
};
