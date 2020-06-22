// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TickingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ALEXARPG_API ATickingProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		float duration;

	UPROPERTY(EditAnywhere)
		float tickFrequency;

	float remainingTime;



	FTimerHandle TickTimer;

	TArray<ARPGCharacter*> targets;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void tick();
};
