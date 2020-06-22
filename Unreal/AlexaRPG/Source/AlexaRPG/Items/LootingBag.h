// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "LootingBag.generated.h"

/**
 *  The class for the item that contain the items dropped when a character died
 */
UCLASS()
class ALEXARPG_API ALootingBag : public AItem
{
	GENERATED_BODY()


protected:
    //  The user interface used to interact with the lootingBag
    TSubclassOf<class UUserWidget> LootUIClass;

    //  Say if the UI file has been correctly loaded
    bool succeded;
public:
    ALootingBag();

    /**
    *	Called when this item get picked (open the UI)
    *	@param charater - The charater which have picked this item
    */
    virtual bool Pick(ARPGCharacter* character) override;

    //  The items contain in the bag
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
        TArray<AItem*> Items;

    //  The maximum number of items that the bag can contain
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
        int MaxItems = 6;

    //  The money contain in the bag
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
        int Coins = 0;

    /**
    *	add money to the lootingBag, the	money will not exceed the maximum money
    *	@param	item - the amount off money to add, the method will do nothing if this parameter is negative
    */
    UFUNCTION(BlueprintCallable)
        void AddCoins(int amount);

    /**
    *	Get the item in a specific slot
    *	@param	pos - The position of the slot
    *	@return	the item in the slot, nullptr if the slot does not exist or if their is no item in the slot
    */
    UFUNCTION(BlueprintCallable)
        bool AddItem(AItem* item);

    /**
    *	Remove an item from the lootingBag
    *	@param	item - the item to remove
    */
    UFUNCTION(BlueprintCallable)
        void removeItem(AItem* item);

    /**
    *	Get the item in a specific slot
    *	@param	pos - The position of the slot
    *	@return	the item in the slot, nullptr if the slot does not exist or if their is no item in the slot
    */
    UFUNCTION(BlueprintCallable)
        AItem* GetItem(int pos);
};
