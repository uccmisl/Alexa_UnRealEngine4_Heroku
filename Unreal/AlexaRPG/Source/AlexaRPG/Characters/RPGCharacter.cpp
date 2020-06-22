// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AlexaRPG/Items/Weapon.h"
#include "Engine/World.h"
#include "Async/Async.h"
#include "UObject/ConstructorHelpers.h"
#include "AlexaRPG/HealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassFinder(TEXT("/Game/AlexaRPG/Blueprints/UI/HPBar"));

	if (MenuClassFinder.Succeeded()) {
		HealthWidget->SetWidgetClass(MenuClassFinder.Class);
	}

	HealthWidget->SetupAttachment((USceneComponent*)GetMesh());

	HealthWidget->SetDrawSize(FVector2D(150, 20));

}


void ARPGCharacter::RotateWidget() {
	HealthWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()));
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = 0.5*MaxHealth;
	CurrentMana = 0.5*MaxMana;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARPGCharacter::OnOverlapBegin);

	if (Weapon != nullptr) {
		Weapon->IsPickable = false;
		Weapon->AttachToComponent((USceneComponent*)GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "hand_rSocket");
		Weapon->WeaponOwner = this;
		Weapon->ChangeVisibility(true);
	}

	inventory = NewObject<UInventory>();

	GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &ARPGCharacter::Regen, 1, true);

	GetWorld()->GetTimerManager().SetTimer(damageTimer, this, &ARPGCharacter::tickDamageSource, 0.1, true);
	

	((UHealthBar*)HealthWidget->GetUserWidgetObject())->character = this;
}

void ARPGCharacter::Tick(float DeltaTime) {
	RotateWidget();
}

void ARPGCharacter::Regen() {
	if (CurrentHealth > 0) {
		CurrentHealth += HealthRegen;
		if (CurrentHealth > MaxHealth) {
			CurrentHealth = MaxHealth;
		}

		CurrentMana += ManaRegen;
		if (CurrentMana > MaxMana) {
			CurrentMana = MaxMana;
		}
	}
}


/////	Mana and Health	/////

void ARPGCharacter::Heal(float amount) {
	if (amount < 0) return;
	CurrentHealth += amount;
	if (CurrentHealth > MaxHealth){
		CurrentHealth = MaxHealth;
	}
}

void ARPGCharacter::TakeDamage_C(float damage, ARPGCharacter* damageDealer, TScriptInterface<IDamageSource> domageSource) {
	if (damage < 0) return;
	if (CurrentHealth <= 0) return;
	
	if (!damageSources.Contains((IDamageSource*)domageSource.GetObject())) {
		if (!domageSource->canDealDomageMultipleTimes()) {
			damageSources.Add((IDamageSource*)domageSource.GetObject(), 1.0);
			GetWorld()->GetTimerManager().UnPauseTimer(damageTimer);
		}
		CurrentHealth -= damage;
		if (CurrentHealth <= 0) {
			CurrentHealth = 0;
			Die(damageDealer);
		}
	}
}

bool ARPGCharacter::ConsumeMana(float amount) {
	if (amount < 0) return false;
	if (CurrentMana < amount) return false;
	CurrentMana -= amount;
	return true;
}

void ARPGCharacter::RestorMana(float amount) {
	if (amount < 0) return;
	CurrentMana += amount;
	if (CurrentMana > MaxMana) {
		CurrentMana = MaxMana;
	}
}

////////////////////////////////////////////


/////	Inventory and equipment	/////

void ARPGCharacter::EquipWeapon(AWeapon* aweapon) {
	if (!UnequipWeapon()) return;
	inventory->RemoveItem(aweapon);
	aweapon->IsPickable = false;
	Weapon = aweapon;
	Weapon->AttachToComponent((USceneComponent*)GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "hand_rSocket");
	Weapon->WeaponOwner = this;
	aweapon->ChangeVisibility(true);
}

bool ARPGCharacter::UnequipWeapon() {
	if (Weapon == nullptr) return true;
	if (!inventory->AddItem(Weapon)) return false;
	Weapon->WeaponOwner = nullptr;
	Weapon = nullptr;
	return true;
}

bool ARPGCharacter::PickItem(AItem* item) {
	return item->Pick(this);
}

////////////////////////////////////////////




/////	Basic attack and abilities	/////

void ARPGCharacter::BasicAttack() {
	if (!IsBasicAttacking) {
		GetWorld()->GetTimerManager().SetTimer(timerBasicAttack, this, &ARPGCharacter::StopBasicAttack, BasicAttackDuration, false);
		IsBasicAttacking = true;
		if (Weapon != nullptr) {
			Weapon->CanDealDamage = true;
		}
	}
}

void ARPGCharacter::StopBasicAttack() {
	IsBasicAttacking = false;
	if (Weapon != nullptr) {
		Weapon->CanDealDamage = false;
	}
}

////////////////////////////////////////////


/////	Damage taking and death	/////

void ARPGCharacter::Die(ARPGCharacter* killer) {
	IsDead = true;
	Async(EAsyncExecution::TaskGraph, [&] { disableMovement(); });
	OnDieEvent();
}

void ARPGCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* weapon = Cast<AWeapon>(OtherActor);
	if (weapon != nullptr) {
		if (weapon->WeaponOwner == this) return;
		if (weapon->CanDealDamage) {
			TakeDamage_C(weapon->BaseDamage + weapon->WeaponOwner->baseDamage, weapon->WeaponOwner, weapon);
		}
	}
}

void  ARPGCharacter::disableMovement()
{
	while(GetMovementComponent()->IsFalling())
	{
		FPlatformProcess::Sleep(0.1);
	}
	((UCharacterMovementComponent*)GetMovementComponent())->DisableMovement();
}

////////////////////////////////////////////



/////	Skills	/////

void ARPGCharacter::AddSkill(TSubclassOf<USkill> skillClass) {
	if (Skills.Num() == NbSkills) return;
	USkill* aSkill = NewObject<USkill>(GetTransientPackage(), skillClass);
	aSkill->Caster = this;
	Skills.Add(aSkill);
}

USkill* ARPGCharacter::GetSkill(int pos) {
	if (Skills.IsValidIndex(pos))
		return Skills[pos];
	return nullptr;
}

//////////////////////////////////////////////////////////



/////	Buffs	/////

void ARPGCharacter::AddBuff(UBuff* buff) {
	for (auto abuff : Buffs) {
		if(abuff->GetClass() == buff->GetClass()){
			buff->Update(buff);
			return;
		}
	}
	Buffs.Add(buff);
	buff->Apply(this);
}

void ARPGCharacter::RemoveBuff(UBuff* buff) {
	if(Buffs.Contains(buff)){
		Buffs.Remove(buff);
	}
}

//////////////////////////////////////////////////////////

void ARPGCharacter::tickDamageSource() {
	if (damageSources.Num() == 0) {
		GetWorld()->GetTimerManager().PauseTimer(damageTimer);
	}

	for (auto& Elem : damageSources) {
		Elem.Value -= 0.1;
		if (Elem.Value < 0) {
			damageSources.Remove(Elem.Key);
		}
	}

	
}