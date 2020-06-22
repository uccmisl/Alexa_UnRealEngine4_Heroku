// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageSource.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALEXARPG_API IDamageSource
{
	GENERATED_BODY()

public:
	virtual bool canDealDomageMultipleTimes();
};
