// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StackableItem.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API AStackableItem : public AItem
{
	GENERATED_BODY()


protected:

	//The number of item in this stack
	int currentStack = 1;

	//The maximum number of items you can stack together
	UPROPERTY(EditAnywhere)
		int MaxStack = 10;

public:
	/**
	*	Called when the item is used from the inventory
	*	@param itemOwner - The character which is using this item
	*/
	virtual void Use(ARPGCharacter* itemOwner) override;

	/**
	*	remove a stack
	*	@return the remaining number of stacks or -1 if their is no stack to remove
	*/
	int removeStack();

	/**
	*	merge two stackable item if they have the same class (transfer the stacks of other to this) (destroy other if all the stacks can be transfered
	*	@return nullptr if all the stacks of other have been transfered or if other is null, other their is still stacks in other
	*/
	AStackableItem* merge(AStackableItem* other);

	//Return a String that describe the Item
	virtual FString toString() override;
};
