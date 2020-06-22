// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StackableItem.h"
#include "RegenPotion.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API ARegenPotion : public AStackableItem
{
	GENERATED_BODY()
	


	virtual void Use(ARPGCharacter* itemOwner) override;
};
