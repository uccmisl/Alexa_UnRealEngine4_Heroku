// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Projectiles/Projectile.h"
#include "Fireball.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API UFireball : public USkill
{
	GENERATED_BODY()


public:
	UFireball();


	TSubclassOf<AProjectile> ProjectileClass;

	virtual void DoCast() override;
};
