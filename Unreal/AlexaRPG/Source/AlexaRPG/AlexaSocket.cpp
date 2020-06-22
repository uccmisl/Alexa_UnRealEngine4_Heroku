// Fill out your copyright notice in the Description page of Project Settings.


#include "AlexaSocket.h"
#include "Skills/Skill.h"
#include "AlexaRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlexaSocket::AAlexaSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("init socket"));
	SIOClientComponent = CreateDefaultSubobject<USocketIOClientComponent>(TEXT("SocketIOClientComponent"));
	SIOClientComponent->AddressAndPort = TEXT("http://alexaunrealserver.herokuapp.com/unreal");
	SIOClientComponent->bShouldAutoConnect = true;
}

// Called when the game starts or when spawned
void AAlexaSocket::BeginPlay()
{
	Super::BeginPlay();
	APlayableCharacter* playerref = player;
	SIOClientComponent->OnNativeEvent(TEXT("message"), [playerref](const FString& Event, const TSharedPtr<FJsonValue>& Message) {
		//Called when the event is received. We can e.g. log what we got
		FString type = USIOJConvert::ToJsonObject(USIOJConvert::ToJsonString(Message))->GetStringField("type");
		FString data = USIOJConvert::ToJsonObject(USIOJConvert::ToJsonString(Message))->GetStringField("data");
		if (playerref != nullptr) {
			if (type == "Cast") {
				for (USkill* skill : playerref->Skills) {
					if (skill->Name == data) {
						skill->Cast();
					}
				}
			}
			else if (type == "Inventory") {
				playerref->OpenInventory();
			}
		}
	});

	SIOClientComponent->OnNativeEvent(TEXT("connected"), [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) {
		//Called when the event is received. We can e.g. log what we got
		UE_LOG(LogTemp, Warning, TEXT("Session Id: %s"), *USIOJConvert::ToJsonString(Message));
		Cast<AAlexaRPGGameModeBase>(GetWorld()->GetAuthGameMode())->sessionId = FCString::Atoi(*(USIOJConvert::ToJsonString(Message)));
	});

	SIOClientComponent->OnNativeEvent(TEXT("alexaConnection"), [](const FString& Event, const TSharedPtr<FJsonValue>& Message) {
		//Called when the event is received. We can e.g. log what we got
		UE_LOG(LogTemp, Warning, TEXT("Connection of the Alexa user: %s"), *USIOJConvert::ToJsonString(Message));
	});

	SIOClientComponent->OnNativeEvent(TEXT("alexaDisconnection"), [this](const FString& Event, const TSharedPtr<FJsonValue>& Message) {
		//Called when the event is received. We can e.g. log what we got
		UE_LOG(LogTemp, Warning, TEXT("Disconnection of the Alexa user: %s"), *USIOJConvert::ToJsonString(Message));
		Cast<AAlexaRPGGameModeBase>(GetWorld()->GetAuthGameMode())->sessionId = 0;
	});
}

