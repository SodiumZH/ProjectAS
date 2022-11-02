
#include "NaItemEffect.h"

FNaItemUsageReturn FNaItemUsageReturn::SetSucceeded(bool value) {
	bSucceeded = value;
	return *this;
}

FNaItemUsageReturn FNaItemUsageReturn::SetConsumedAmount(int value) {
	ConsumedAmount = value;
	return *this;
}

FNaItemUsageReturn FNaItemUsageReturn::Null(){
	return FNaItemUsageReturn();
}

FNaItemUsageReturn FNaItemUsageReturn::Consumed(){
	return FNaItemUsageReturn().SetSucceeded(true).SetConsumedAmount(1);
}

FNaItemUsageReturn FNaItemUsageReturn::UsedNoConsumption() {
	return FNaItemUsageReturn().SetSucceeded(true);
}

FNaItemUsageReturn UNaItemEffect::ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	return FNaItemUsageReturn::Null();
}

FNaItemUsageReturn UNaItemEffect::ItemEffectBP_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	return FNaItemUsageReturn::Null();
}

FNaItemUsageReturn UNaItemEffect::UseItem(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	checkf(IsValid(SourceActor), TEXT("UseItem error: Invalid source actor."));
	if (Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->bUseBlueprintOverride) {
		return Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->ItemEffectBP(ItemID, SourceActor, TargetActor, ItemPosition);
	}
	else {
		return Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->ItemEffect(ItemID, SourceActor, TargetActor, ItemPosition);
	}
}