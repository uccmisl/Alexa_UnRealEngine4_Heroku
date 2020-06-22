// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/Widget.h"
#include "AlexaRPG/Items/LootingBag.h"
#include "PlayableCharacter.generated.h"

/**
 * The base class for every characters the player will be able to control
 */
UCLASS()
class ALEXARPG_API APlayableCharacter : public ARPGCharacter
{
	GENERATED_BODY()


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Bind input key's
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	/////	Movements	/////

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	//The timer used to reset the jump state variables when the annimation is finished
	FTimerHandle timerJump;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/** Called for jump input */
	void StartJump();

	//	Reset the jump state variables
	void onTimerEnd();

	////////////////////////////////////////



	/////	Experience	and leveling	/////

	/*
	*	Add a level to the player
	*	the level will not exceed the maximum level of the player
	*	update the stats of the player
	*/
	void LvlUp();

	////////////////////////////////////////



	/////	Inventory and interactions	/////

	//	All the items in range
	TArray<AItem*> OverlapedItems;

	//	The current targeted item
	AItem* ClosestItem=nullptr;

	//The timer used to update the current targeted item every eriod of time
	FTimerHandle timerItemUpdate;

	//	Interact with the current targeted item
	void Interact();

	//	Update the current targeted item to the closest item in range
	void closestItem();

	//	Called when another actor enter the range of the player
	UFUNCTION()
		void OnOverlapBeginItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//	Called when another actor exit the range of the player
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	////////////////////////////////////////



public:
	APlayableCharacter();



	/////	Movements	/////

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//	Say if the player is jumping
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool StartJumping;

	////////////////////////////////////////



	/////	Experience	and leveling	/////

	//	The level of the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentLvl;

	//	The maximum level of the player
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxLvl;

	//	The experience of the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentExp;

	//	The experience level of the player
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxExp;

	////	The parameters that will define how the amount of experience needed to reach the next level will evolve folowing an affine function
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ExpScalingFactor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ExpScalingAdd;

	////	The parameters that will define what amount of MaxHealth will be gain when reaching the next level folowing an affine function
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float HealthScalingFactor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float HealthScalingAdd;

	////	The parameters that will define what amount of MaxMana will be gain when reaching the next level folowing an affine function
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ManaScalingFactor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ManaScalingAdd;


	/**
	 *	Add experience to the player, make him level up if needed
	 *	@param exp - the amount of experience to add, the method will do nothing if this parameter is negative
	 */
	UFUNCTION(BlueprintCallable)
		void GainExp(float exp);

	////////////////////////////////////////



	/////	Inventory and  interactions	/////

	//	Implemented in blueprint to open the inventory UI
	UFUNCTION(BlueprintImplementableEvent)
		void OpenInventory();

	//	The range in wich the player can pick items
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Range;

	//	The updating rate of the current targeted ite
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PickingTargetUpdateTime = 1;

	// The looting bag the player is picking
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ALootingBag* Loots;

	////////////////////////////////////////



	/////	Skills	/////

	//	These methods are used to bind skills keys
	void cast(int pos);
	void cast1();
	void cast2();
	void cast3();
	void cast4();
	void cast5();
	void cast6();
	void cast7();
	void cast8();
	void cast9();
	void cast0();

	////////////////////////////////////////
};
