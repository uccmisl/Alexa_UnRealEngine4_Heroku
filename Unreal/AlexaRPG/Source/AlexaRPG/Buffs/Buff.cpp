// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"
#include "AlexaRPG/Characters/RPGCharacter.h"


void UBuff::Update(UBuff* buff) {
	RemainingTime = Duration;
}

void UBuff::Apply(ARPGCharacter* target) {
	if (target != nullptr) {
			RemainingTime = Duration;
			target->GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &UBuff::EndBuff, UpdateTime, true);
			Target = target;
	}
}

void UBuff::EndBuff() {
	RemainingTime -= UpdateTime;
	if (RemainingTime <= 0) {
		RemainingTime = 0;
		DoEndBuff();
		Target->RemoveBuff(this);
		Target->GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
	}
}

void  UBuff::DoEndBuff(){}