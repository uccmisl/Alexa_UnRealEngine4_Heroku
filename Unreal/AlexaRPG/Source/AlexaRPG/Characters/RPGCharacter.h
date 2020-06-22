// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlexaRPG/Inventory.h"
#include "AlexaRPG/Items/Item.h"
#include "AlexaRPG/Skills/Skill.h"
#include "AlexaRPG/Buffs/Buff.h"
#include "AlexaRPG/DamageSource.h"
#include "RPGCharacter.generated.h"

class AWeapon;

UCLASS()
/*
* The base class for every characters in the game 
*/
class ALEXARPG_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/////	Mana and Health	/////

	//The timer which will call the regen method every seconds
	FTimerHandle RegenTimer;

	//Update health and mana when the character regenerates
	void Regen();

	//////////////////////////////////////////////////////////



	/////	Basic attack and abilities	/////

	//The timer used to reset the basic attack state variables when the annimation is finished
	FTimerHandle timerBasicAttack;

	//Reset the basic attack state variables
	void StopBasicAttack();

	//////////////////////////////////////////////////////////



	/////	Damage taking and death	/////

	// Called when the character die
	virtual void Die(ARPGCharacter* killer);

	//	Called when another actor overlap the hitbox of the character
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//////////////////////////////////////////////////////////



public:

	/////	Mana and Health	/////

	//The maximum health of the character
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MaxHealth;

	//The maximum Mana offthe character
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float MaxMana;

	//The Mana off the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentHealth;


	//The Mana off the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentMana;

	//The amount of Health regenerated every seconds
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float HealthRegen;

	//The amount of Health regenerated every seconds
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ManaRegen;

	/**
	*	Heal the character by a certain amount, the health will not exceed the maximum health
	*	@param amount - The amount of health healed, the method will do nothing if this parameter is negative
	*/
	UFUNCTION(BlueprintCallable)
		void Heal(float amount);

	/**
	*	Deal domage to the character by a certain amount and launch death event if needed, the health will not take values under 0
	*	@param amount - The amount of damage dealt, the method will do nothing if this parameter is negative
	*/
	UFUNCTION(BlueprintCallable)
		void TakeDamage_C(float damage, ARPGCharacter* damageDealer, TScriptInterface<IDamageSource> domageSource);

	/**
	*	Restore character's Mana by a certain amount, the mana will not exceed the maximum mana
	*	@param amount - The amount of mana restored, the method will do nothing if this parameter is negative
	*/
	UFUNCTION(BlueprintCallable)
		void RestorMana(float amount);

	/**
	*	Conume character's mana, by a certain amount if the player have enough mana, the mana will not take values under 0
	*	@param amount - The amount of man consumed, the method will do nothing if this parameter is negative
	*	@return true if the player have enough mana, false if not
	*/
	UFUNCTION(BlueprintCallable)
		bool ConsumeMana(float amount);

	// The widget used to notify the player that he can pick this item
	UPROPERTY(VisibleAnywhere)
		UWidgetComponent* HealthWidget;

	//Turn the Health Widget towards the player
	void RotateWidget();

	////////////////////////////////////////////////



	/////	Inventory and equipment	/////

	// The weapon with which the character is equipped
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AWeapon* Weapon;

	//	The player's Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInventory* inventory;

	/**
	*	Equip the character with a weapon after unequiped him if he already had one
	*	@param weapon - The weapon to equip
	*/
	UFUNCTION(BlueprintCallable)
		void EquipWeapon(AWeapon* aweapon);

	//	Unequip the character's weapon if he has one
	UFUNCTION(BlueprintCallable)
		bool UnequipWeapon();

	//	Pick an item wich is on the ground
	UFUNCTION(BlueprintCallable)
		bool PickItem(AItem* item);

	////////////////////////////////////////////////////



	/////	Basic attack and abilities	/////

	// The base damage of the charactr
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float baseDamage;

	//	Say if the character is attacking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsBasicAttacking=false;

	//	The duration of the basic attack animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float BasicAttackDuration;

	//	Start Basic attack animation and start the timer
	UFUNCTION(BlueprintCallable)
		void BasicAttack();



	/////	Damage taking and death	/////

	//	Can be implemented in Blueprint to add some interactions to the Die method
	UFUNCTION(BlueprintImplementableEvent)
		void OnDieEvent();

	//	Say if the character is dead
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsDead=false;

	//	Stop character's movement and prevent it from moving (if the character was falling, wait until he hit the ground)
	void disableMovement();

	TMap<IDamageSource*, float> damageSources;

	FTimerHandle damageTimer;

	void tickDamageSource();

	//////////////////////////////////////////////////////////


	/////	Skills	/////

	//	The character's Skills
	UPROPERTY(BlueprintReadWrite)
		TArray<USkill*> Skills;

	//	Create a skill from a class and add it to the character's Skills if a slot is available
	UFUNCTION(BlueprintCallable)
		void AddSkill(TSubclassOf<USkill> skillClass);

	//	The number of Skill slots the character have
	UPROPERTY(EditAnywhere)
		int NbSkills = 10;

	//	Get the Skill in a specific slot
	UFUNCTION(BlueprintCallable)
		USkill* GetSkill(int pos);

	//////////////////////////////////////////////////////////




	/////	Buffs	/////

	//	The Buffs that affect the character
	TArray<UBuff*> Buffs;

	//	Apply a new or Update the timer the timer of a buff on this character
	UFUNCTION(BlueprintCallable)
		void AddBuff(UBuff* buff);

	//	Remove a buff
	void RemoveBuff(UBuff* buff);

	//////////////////////////////////////////////////////////
};

