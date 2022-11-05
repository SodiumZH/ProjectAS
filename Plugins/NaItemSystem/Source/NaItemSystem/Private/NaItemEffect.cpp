
#include "NaItemEffect.h"
#include "BPLibraries/NaItemDataStatics.h"

ENaItemContainerUsageResult ENaItemContainerUsageResult::SetSucceeded(bool value) {
	bSucceeded = value;
	return *this;
}

ENaItemContainerUsageResult ENaItemContainerUsageResult::SetConsumedAmount(int value) {
	ConsumedAmount = value;
	return *this;
}

ENaItemContainerUsageResult ENaItemContainerUsageResult::Null(){
	return ENaItemContainerUsageResult();
}

ENaItemContainerUsageResult ENaItemContainerUsageResult::Consumed(){
	return ENaItemContainerUsageResult().SetSucceeded(true).SetConsumedAmount(1);
}

ENaItemContainerUsageResult ENaItemContainerUsageResult::UsedNoConsumption() {
	return ENaItemContainerUsageResult().SetSucceeded(true);
}

//ENaItemContainerUsageResult UNaItemEffect::ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
//	return ENaItemContainerUsageResult::Null();
//}

ENaItemContainerUsageResult UNaItemEffect::ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	return ENaItemContainerUsageResult::Null();
}

ENaItemContainerUsageResult UNaItemEffect::UseItem(UObject* WorldContext, int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	
	// Source actor is not allowed to be null
	checkf(IsValid(SourceActor), TEXT("UseItem error: Invalid source actor. Using item ID: %d"), ItemID);

	// When ID is invalid (including empty), do nothing
	if (!UNaItemDataStatics::GetItemTypeFromID(WorldContext, ItemID).IsValidType()) {
		return ENaItemContainerUsageResult::Null();
	}
	// If the item is not usable, do nothing
	if (!UNaItemDataStatics::GetItemEffectDataFromID(WorldContext, ItemID).bCanUse) {
		return ENaItemContainerUsageResult::Null();
	}
	// Blueprint override
	//if (bUseBlueprintOverride) {
		return ItemEffect(ItemID, SourceActor, TargetActor, ItemPosition);
	//}
	// C++ override
//	else {
//		return Cast<UNaItemEffect>(StaticClass()->GetDefaultObject())->ItemEffect(ItemID, SourceActor, TargetActor, ItemPosition);
//	}
}