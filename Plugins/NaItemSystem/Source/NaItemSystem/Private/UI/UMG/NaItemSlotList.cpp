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
	
	if (!IsValid(ContainerComponent)) {
		UE_LOG(LogNaItem, Display, TEXT("UNaItemSlotList: invalid container reference."));
		SAssignNew(List, SNaItemSlotList);
		return List.ToSharedRef();
	}

	SAssignNew(List, SNaItemSlotList)
		.Container(ContainerComponent);
	List->PostConstructionInit();

	// Bind click actions
	


	return List.ToSharedRef();
	
}

void UNaItemSlotList::SetContainerComponent(UNaItemContainerComponent* NewComponent) {
	ContainerComponent = NewComponent;
	if(List.IsValid())
		List->ResetContainer(NewComponent);
}

void UNaItemSlotList::Refresh() {
	if(List.IsValid())
		List->ResetAllSlots();
}


