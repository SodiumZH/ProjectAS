
#include "NaItemEffect.h"
#include "BPLibraries/NaItemDataStatics.h"

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

FNaItemUsageReturn UNaItemEffect::UseItem(UObject* WorldContext, int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	
	// Source actor is not allowed to be null
	checkf(IsValid(SourceActor), TEXT("UseItem error: Invalid source actor. Using item ID: %d"), ItemID);

	// When ID is invalid (including empty), do nothing
	if (!UNaItemDataStatics::GetItemTypeFromID(WorldContext, ItemID).IsValidType()) {
		return FNaItemUsageReturn::Null();
	}
	// If the item is not usable, do nothing
	if (!UNaItemDataStatics::GetItemEffectDataFromID(WorldContext, ItemID).bCanUse) {
		return FNaItemUsageReturn::Null();
	}
	// Blueprint override
	if (Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->bUseBlueprintOverride) {
		return Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->ItemEffectBP(ItemID, SourceActor, TargetActor, ItemPosition);
	}
	// C++ override
	else {
		return Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->ItemEffect(ItemID, SourceActor, TargetActor, ItemPosition);
	}
}