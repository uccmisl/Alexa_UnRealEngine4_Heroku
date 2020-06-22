// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

APlayableCharacter::APlayableCharacter() : ARPGCharacter()
{
	HealthWidget->SetVisibility(false);

	Range = CreateDefaultSubobject<USphereComponent>(TEXT("Range"));
	Range->SetCollisionProfileName("OverlapAll");
	Range->SetRelativeScale3D(FVector(1, 1, 1));
	Range->SetSphereRadius(300);

	Range->SetupAttachment(RootComponent);


	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	Range->OnComponentBeginOverlap.AddDynamic(this, &APlayableCharacter::OnOverlapBeginItem);
	Range->OnComponentEndOverlap.AddDynamic(this, &APlayableCharacter::OnOverlapEnd);
}

// Input

void APlayableCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayableCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayableCharacter::Interact);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APlayableCharacter::OpenInventory);
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &APlayableCharacter::BasicAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayableCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &APlayableCharacter::cast1);
	PlayerInputComponent->BindAction("Ability2", IE_Pressed, this, &APlayableCharacter::cast2);
	PlayerInputComponent->BindAction("Ability3", IE_Pressed, this, &APlayableCharacter::cast3);
	PlayerInputComponent->BindAction("Ability4", IE_Pressed, this, &APlayableCharacter::cast4);
	PlayerInputComponent->BindAction("Ability5", IE_Pressed, this, &APlayableCharacter::cast5);
	PlayerInputComponent->BindAction("Ability6", IE_Pressed, this, &APlayableCharacter::cast6);
	PlayerInputComponent->BindAction("Ability7", IE_Pressed, this, &APlayableCharacter::cast7);
	PlayerInputComponent->BindAction("Ability8", IE_Pressed, this, &APlayableCharacter::cast8);
	PlayerInputComponent->BindAction("Ability9", IE_Pressed, this, &APlayableCharacter::cast9);
	PlayerInputComponent->BindAction("Ability0", IE_Pressed, this, &APlayableCharacter::cast0);

}

//////////////////////////////////////////////////////////////////////////


/////	Movements	/////

void APlayableCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayableCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayableCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void  APlayableCharacter::StartJump()
{
	if (!GetCharacterMovement()->IsFalling()) {
		StartJumping = true;
		GetWorld()->GetTimerManager().SetTimer(timerJump, this, &APlayableCharacter::onTimerEnd, 0.4f, false);
		GetWorld()->GetTimerManager().SetTimer(timerItemUpdate, this, &APlayableCharacter::closestItem, PickingTargetUpdateTime, true);
	}
}

void APlayableCharacter::onTimerEnd() {
	StartJumping = false;
	Jump();
}

////////////////////////////////////////



/////	Experience	and leveling	/////

void APlayableCharacter::LvlUp() {
	if (CurrentLvl < MaxLvl) {
		CurrentLvl++;
		MaxExp = ExpScalingFactor * MaxExp + ExpScalingAdd;
		float HealthScaling = (HealthScalingFactor-1) * MaxHealth + HealthScalingAdd;
		MaxHealth += HealthScaling;
		CurrentHealth += HealthScaling;
		float ManaScaling = (ManaScalingFactor-1) * MaxMana + ManaScalingAdd;
		MaxMana += ManaScaling;
		CurrentMana += ManaScaling;
		if (CurrentLvl == MaxLvl) {
			CurrentExp = 1;
			MaxExp = 1;
		}
	}
}

void APlayableCharacter::GainExp(float exp) {
	if (exp < 0) return;
	if (CurrentHealth > 0) {
		if (CurrentLvl < MaxLvl) {
			CurrentExp += exp;
			if (CurrentExp >= MaxExp) {
				CurrentExp -= MaxExp;
				LvlUp();
			}
		}
	}
}

////////////////////////////////////////



/////	Inventory and  interactions	/////

void APlayableCharacter::OnOverlapBeginItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AItem* item = Cast<AItem>(OtherActor);
	if (item != nullptr) {
		if (item->IsPickable) {
			OverlapedItems.Add(item);
			closestItem();
		}
	}
}

void APlayableCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AItem* item = Cast<AItem>(OtherActor);
	if (item != nullptr) {
		if (item->IsPickable) {
			OverlapedItems.Remove(item);
			closestItem();
		}
	}
}

void APlayableCharacter::Interact() {

	if (ClosestItem != nullptr) {
		PickItem(ClosestItem);
		if (OverlapedItems.Contains(ClosestItem)) {
			OverlapedItems.Remove(ClosestItem);
		}
		closestItem();
	}
}

void APlayableCharacter::closestItem() {

	if (ClosestItem != nullptr) {
		ClosestItem->showInteraction(false);
	}
	int nb = OverlapedItems.Num();
	if (nb == 0) {
		ClosestItem = nullptr;
		return;
	}
	ClosestItem = OverlapedItems[0];
	for (int i = 1; i < nb; ++i) {
		if ((OverlapedItems[i]->GetActorLocation() - GetActorLocation()).SizeSquared() < (ClosestItem->GetActorLocation() - GetActorLocation()).SizeSquared()) {
			ClosestItem = OverlapedItems[i];
		}
	}
	ClosestItem->showInteraction(true);
}

////////////////////////////////////////


/////	Skills	/////
void APlayableCharacter::cast(int pos) {
	if (Skills.IsValidIndex(pos)) {
		if (Skills[pos] != nullptr) {
			Skills[pos]->Cast();
		}
	}
}
void APlayableCharacter::cast1() {
	UE_LOG(LogTemp, Warning, TEXT("Cast1"));
	cast(0);
}
void APlayableCharacter::cast2() {
	UE_LOG(LogTemp, Warning, TEXT("Cast2"));
	cast(1);
}
void APlayableCharacter::cast3() {
	UE_LOG(LogTemp, Warning, TEXT("Cast3"));
	cast(2);
}
void APlayableCharacter::cast4() {
	UE_LOG(LogTemp, Warning, TEXT("Cast4"));
	cast(3);
}
void APlayableCharacter::cast5() {
	UE_LOG(LogTemp, Warning, TEXT("Cast5"));
	cast(4);
}
void APlayableCharacter::cast6() {
	UE_LOG(LogTemp, Warning, TEXT("Cast6"));
	cast(5);
}
void APlayableCharacter::cast7() {
	UE_LOG(LogTemp, Warning, TEXT("Cast7"));
	cast(6);
}
void APlayableCharacter::cast8() {
	UE_LOG(LogTemp, Warning, TEXT("Cast8"));
	cast(7);
}
void APlayableCharacter::cast9() {
	UE_LOG(LogTemp, Warning, TEXT("Cast9"));
	cast(8);
}
void APlayableCharacter::cast0() {
	UE_LOG(LogTemp, Warning, TEXT("Cast0"));
	cast(9);
}
////////////////////////////////////////