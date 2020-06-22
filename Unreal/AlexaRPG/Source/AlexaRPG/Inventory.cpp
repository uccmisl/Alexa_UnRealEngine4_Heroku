// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Items/StackableItem.h"

UInventory::UInventory(){

}

bool UInventory::AddItem(AItem* item) {
	AStackableItem* stackItem = Cast<AStackableItem>(item);
	int i;
	if (stackItem != nullptr) {
		for (i = 0; i < maxSize; ++i) {
			AItem* otherItem = GetItem(i + 1);
			if (otherItem != nullptr && otherItem->GetClass() == stackItem->GetClass()) {
				item = ((AStackableItem*)otherItem)->merge(stackItem);
				if (item == nullptr) return true;
			}
		}
	}

	if (Items.FindKey(item) != nullptr) return false;

	for (i = 0; i < maxSize; ++i) {
		if (!Items.Contains(i+1)) {
			Items.Add(i + 1, item);
			item->IsPickable = false;
			item->ChangeVisibility(false);
			return true;
		}
		else {
			
		}
	}
	return false;
}

AItem* UInventory::GetItem(int pos) {
	if (Items.Contains(pos)) {
		return Items[pos];
	}
	return nullptr;
}

void  UInventory::RemoveItem(AItem* item) {
	const int* key = Items.FindKey(item);
	if (key != nullptr) {
		Items.Remove(*key);
		const int* key2 = Items.FindKey(item);
		if (key2 != nullptr) {
		}
	}
}

void UInventory::AddCoins(int nb) {
	if (nb < 0) return;
	Coins += nb;
	if (Coins > MaxCoins) {
		Coins = MaxCoins;
	}
}

bool UInventory::RemoveCoins(int nb) {
	if (nb < 0) return false;
	if (Coins < nb) return false;
	Coins -= nb;
	return true;
}