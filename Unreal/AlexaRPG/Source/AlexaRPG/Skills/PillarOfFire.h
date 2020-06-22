// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Projectiles/TickingProjectile.h"
#include "PillarOfFire.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API UPillarOfFire : public USkill
{
	GENERATED_BODY()

public:
	UPillarOfFire();

	TSubclassOf<ATickingProjectile> ProjectileClass;

	virtual void DoCast() override;
};
