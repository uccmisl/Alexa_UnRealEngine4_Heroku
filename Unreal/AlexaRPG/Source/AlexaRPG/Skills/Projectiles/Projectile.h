// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AlexaRPG/Characters/RPGCharacter.h"
#include "AlexaRPG/DamageSource.h"
#include "Projectile.generated.h"

UCLASS()
class ALEXARPG_API AProjectile : public AActor, public IDamageSource
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* particle;

	UPROPERTY(EditAnywhere)
		UParticleSystem* hitParticle;

	UPROPERTY(VisibleAnywhere)
		UShapeComponent* hitBox;

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMovementComponent;

	void SetInitialDirection(const FVector& initialDirection);

	UFUNCTION(BlueprintImplementableEvent)
		void applyEffectsEvent(ARPGCharacter* target);

	virtual void applyEffects(ARPGCharacter* target);

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly)
	ARPGCharacter* Caster = nullptr;
};
