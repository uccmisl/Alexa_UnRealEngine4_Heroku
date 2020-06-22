// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Item.h"
#include "Inventory.generated.h"

/**
 *	The class for the character's inventory
 */
UCLASS()
class ALEXARPG_API UInventory : public UObject
{
	GENERATED_BODY()
	
public:

	UInventory();
	
	//	The items contain in the inventory
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<int,AItem*> Items;

	//	The number of items that the inventory can contain
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int maxSize=16;

	//	The money contains in the inventory
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int Coins = 0;

	//	The maximum money that the inventory can contain
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int MaxCoins = 10000;

	/**
	*	Add an item in the inventory if possible
	*	@param item - The item to add
	*	@return true if the item has been added, false if not
	*/
	UFUNCTION(BlueprintCallable)
		bool AddItem(AItem* item);

	/**
	*	Get the item in a specific slot
	*	@param	pos - The position of the slot
	*	@return	the item in the slot, nullptr if the slot does not exist or if their is no item in the slot
	*/
	UFUNCTION(BlueprintCallable)
		AItem* GetItem(int pos);

	/**
	*	Remove an item from the inventory
	*	@param	item - the item to remove
	*/
	UFUNCTION(BlueprintCallable)
		void RemoveItem(AItem* item);

	/**
	*	add money to the inventory, the	money will not exceed the maximum money
	*	@param	item - the amount off money to add, the method will do nothing if this parameter is negative
	*/
	UFUNCTION(BlueprintCallable)
		void AddCoins(int nb);

	/**
	*	remove money from the inventory if it contain enough money, the money will not take values under 0
	*	@param	item - the amount off money to remove, the method will do nothing if this parameter is negative
	*	@return true if the inventory contain enough money, false if not
	*/
	UFUNCTION(BlueprintCallable)
		bool RemoveCoins(int nb);

};
