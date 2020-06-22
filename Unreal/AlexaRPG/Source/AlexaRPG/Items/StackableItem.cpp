// Fill out your copyright notice in the Description page of Project Settings.


#include "StackableItem.h"
#include "AlexaRPG/Characters/RPGCharacter.h"

FString AStackableItem::toString() {
	return name + " X" + FString::FromInt(currentStack);
}

int  AStackableItem::removeStack() {
	if (currentStack < 0) return -1;
	currentStack--;
	return currentStack;
}

void AStackableItem::Use(ARPGCharacter* itemOwner) {
	UseImplementation(itemOwner);
	if (!removeStack()) {
		itemOwner->inventory->RemoveItem(this);
		this->Destroy();
	}
}

AStackableItem* AStackableItem::merge(AStackableItem* other) {
	if (other == nullptr) return nullptr;
	if (GetClass() != other->GetClass()) return other;
	bool stoploop = false;
	while (currentStack!=MaxStack) {
		if (other->removeStack()<0) {
			other->Destroy();
			return nullptr;
		}
		currentStack++;
	}
	return other;
}