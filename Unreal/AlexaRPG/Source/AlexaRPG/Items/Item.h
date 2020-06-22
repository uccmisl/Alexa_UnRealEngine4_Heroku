// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 	
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Item.generated.h"

class ARPGCharacter;

/**
 *	The base class for every items
 */
UCLASS()
class ALEXARPG_API AItem : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	

	/////	Components	/////

	// The name of the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString name;

	/// The visual Components of the item
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;
	///

	// The hitbox of the item
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* HitBox;

	// The widget used to notify the player that he can pick this item
	UPROPERTY(VisibleAnywhere)
		UWidgetComponent* InteractWidget;

	/////////////////////////////////



	/////	Interactions	/////

	// Say if this item can be pick
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsPickable = true;

	/**
	*	Called when the item is used from the inventory
	*	@param itemOwner - The character which is using this item
	*/
	UFUNCTION(BlueprintCallable)
		virtual void Use(ARPGCharacter* itemOwner);

	//Can be implemented in Blueprint to add some interactions to the Use method
	UFUNCTION(BlueprintImplementableEvent)
		void UseImplementation(ARPGCharacter* itemOwner);

	virtual void ChangeVisibility(bool visibility);

	/**
	*	Update the visibility of the Interaction notification widget
	*	@param isShown - True if you want to make the widget visible, false if not
	*/
	UFUNCTION(BlueprintCallable)
	void showInteraction(bool isShown);

	/**
	*	Called when this item get picked (try to add the item to the character's inventory)
	*	@param charater - The charater which have picked this item
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool Pick(ARPGCharacter* character);

	//Rotate the Interaction widget towards the player
	void RotateWidget();

	/////////////////////////////////

	//Return a String that describe the Item
	UFUNCTION(BlueprintCallable)
		virtual FString toString();
};
