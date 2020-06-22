// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Characters/Monster.h"
#include "Items/Weapon.h"
#include "SpawnerArea.generated.h"

/**
 *	The base class for monster spawners
 */
UCLASS()
class ALEXARPG_API ASpawnerArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//	The timer which will call the spawning method every period of time
	FTimerHandle SpawningTimer;

public:	

	//	The area inside which monsters can spawn
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Area;

	//	The type of monster to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMonster> CharacterToSpawn;

	//	The type of the weapon that the spawned monsters will be equiped
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AItem>> MonsterItems;

	//	The types of the items the spawned monster will have
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeapon> MonsterWeapon;

	//	The maximum number of monsters that can be alive at the same time
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int MaxEntities = 5;

	//	The minimum number of monster to have after when spawning
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int SpawnCap = 3;

	//	The number of monster spawned by this spawner currently alive
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int EntitiesCount = 0;

	//	The spawnnig rate
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SpawningInterval = 10.0f;

	//	Spawn a random number of monster in the area (the number of monster alive after spawning will be between the maximum and minimum values)
	UFUNCTION(BlueprintCallable)
		void SpawnEntities();

};
