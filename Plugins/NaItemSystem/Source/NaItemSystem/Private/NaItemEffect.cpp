
#include "NaItemEffect.h"

FNaItemUsageReturn FNaItemUsageReturn::bSucceeded(bool value) {
	bSucceeded = value;
	return *this;
}

FNaItemUsageReturn FNaItemUsageReturn::ConsumedAmount(int value) {
	ConsumedAmount = value;
	return *this;
}

FNaItemUsageReturn FNaItemUsageReturn::Null(){
	return FNaItemUsageReturn();
}

FNaItemUsageReturn FNaItemUsageReturn::Consumed(){
	return FNaItemUsageReturn().bSucceeded(true).ConsumedAmount(1);
}

FNaItemUsageReturn FNaItemUsageReturn::UsedNoConsumption() {
	return FNaItemUsageReturn().bSucceeded(true);
}

FNaItemUsageReturn UNaItemEffect::ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	return FNaItemUsageReturn::Null();
}

FNaItemUsageReturn UNaItemEffect::ItemEffectBP_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	return FNaItemUsageReturn::Null();
}

FNaItemUsageReturn UNaItemEffect::UseItem(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	if (bUseBlueprintOverride) {
		return ItemEffectBP(ItemID, SourceActor, TargetActor, ItemPosition);
	}
	else {
		return ItemEffect(ItemID, SourceActor, TargetActor, ItemPosition);
	}
}