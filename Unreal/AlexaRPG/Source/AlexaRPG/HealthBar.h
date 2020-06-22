// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/RPGCharacter.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	ARPGCharacter* character;
};
