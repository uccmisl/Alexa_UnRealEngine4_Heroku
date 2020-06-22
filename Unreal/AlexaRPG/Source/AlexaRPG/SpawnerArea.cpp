// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerArea.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASpawnerArea::ASpawnerArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
	Area->SetCollisionProfileName("NoCollision");
	Area->SetBoxExtent(FVector(10, 10, 10));
	RootComponent = Area;

}

// Called when the game starts or when spawned
void ASpawnerArea::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawningTimer, this, &ASpawnerArea::SpawnEntities, SpawningInterval, true);
}

void ASpawnerArea::SpawnEntities() {
	if (CharacterToSpawn != nullptr) {
		if (EntitiesCount < SpawnCap) {
			for (EntitiesCount; EntitiesCount < FMath::RandRange(SpawnCap, MaxEntities); ++EntitiesCount) {
				FVector RandomVector = Area->K2_GetComponentLocation();
				FVector extend = Area->GetScaledBoxExtent();
				RandomVector.X += FMath::RandRange(-extend.X, extend.X);
				RandomVector.Y += FMath::RandRange(-extend.Y, extend.Y);
				RandomVector.Z += FMath::RandRange(-extend.Z, extend.Z);
				AMonster* SpawnedActor = GetWorld()->SpawnActor<AMonster>(CharacterToSpawn, RandomVector, FRotator(0, 0, 0), FActorSpawnParameters());
				SpawnedActor->spawner = this;
				if (MonsterWeapon != nullptr) {
					AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(MonsterWeapon, FVector(0, 0, 0), FRotator(0, 0, 0), FActorSpawnParameters());
					SpawnedActor->EquipWeapon(SpawnedWeapon);
				}
				for (auto& itemClass : this->MonsterItems) {
					AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(itemClass, FVector(0, 0, 0), FRotator(0, 0, 0), FActorSpawnParameters());
					if (!SpawnedActor->PickItem(SpawnedItem)) {
						SpawnedItem->Destroy();
					}
				}
			}
		}
	}
}