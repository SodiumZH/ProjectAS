
#include "NaItemEffect.h"

ENaItemContainerUsageResult UNaItemEffect::ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	return ENaItemContainerUsageResult::ICUR_Failed;
}

ENaItemContainerUsageResult UNaItemEffect::UseItem(UObject* WorldContext, int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) {
	
	// Source actor is not allowed to be null
	checkf(IsValid(SourceActor), TEXT("UseItem error: Invalid source actor. Using item ID: %d"), ItemID);

	// ID == 0, empty item
	if (ItemID == 0) {
		return ENaItemContainerUsageResult::ICUR_Empty;
	}

	return ItemEffect(ItemID, SourceActor, TargetActor, ItemPosition);

}