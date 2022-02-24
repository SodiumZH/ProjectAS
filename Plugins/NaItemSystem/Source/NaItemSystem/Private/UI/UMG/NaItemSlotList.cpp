// By Sodium


#include "UI/UMG/NaItemSlotList.h"
#include "UI/Widgets/SNaItemSlotList.h"
#include "Components/NaItemContainerComponent.h"
#include "NaItemContainer.h"
#include "NaUtilityMinimal.h"

#define LOCTEXT_NAMESPACE "NaItemSystem"

const FText UNaItemSlotList::GetPaletteCategory() {
	return LOCTEXT("NaItemSystem", "NaItemSystem");
}

TSharedRef<SWidget> UNaItemSlotList::RebuildWidget(){
	
	TSharedPtr<FNaItemContainer> Container;
	if (!Container.IsValid()) {
		UE_LOG(LogNaItem, Display, TEXT("UNaItemSlotList: invalid container reference."));
		return SNew(SNaItemSlotList);
	}

	List = SNew(SNaItemSlotList)
		.WorldContext(this)
		.ContainerPtr(Container);

	return List.ToSharedRef();
	
}
