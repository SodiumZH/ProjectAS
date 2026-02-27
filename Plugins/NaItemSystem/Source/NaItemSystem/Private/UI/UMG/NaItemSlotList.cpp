#include "UI/UMG/NaItemSlotList.h"
#include "UI/Widgets/SNaItemSlotList.h"
#include "Components/NaItemInventoryComponent.h"
#include "NatriumItemSystem.h"
#include "NaUtilityMinimal.h"

#define LOCTEXT_NAMESPACE "NaItemSystem"

const FText UNaItemSlotList::GetPaletteCategory() {
	return LOCTEXT("NaItemSystem", "NaItemSystem");
}

TSharedRef<SWidget> UNaItemSlotList::RebuildWidget(){
	
	FNaItemSlotPublicStyle* PublicStylePtr = &PublicStyle;
	if (!IsValid(ContainerComponent)) {
		UE_LOG(LogNaItem, Display, TEXT("UNaItemSlotList: invalid container reference."));
		SAssignNew(List, SNaItemSlotList)
			.FromUMG(this)
			.StylePtr(PublicStylePtr)
			.RowLength(RowLength)
			.RowCount(RowCount);
		return List.ToSharedRef();
	}
	else {
		SAssignNew(List, SNaItemSlotList)
			.Container(ContainerComponent)
			.FromUMG(this)
			.StylePtr(PublicStylePtr)
			.RowLength(RowLength)
			.RowCount(RowCount);
	}

	return List.ToSharedRef();
	
}

void UNaItemSlotList::SetContainerComponent(UNaItemInventoryComponent* NewComponent) {
	ContainerComponent = NewComponent;
	if(List.IsValid())
		List->ResetContainer(NewComponent);
}

void UNaItemSlotList::Refresh() {
	if(List.IsValid())
		List->ResetAllSlots();
}

void UNaItemSlotList::SelectSlot(int Position) {
	if (List.IsValid())
		List->SelectSlot(Position);
}

void UNaItemSlotList::UnselectAll() {
	if (List.IsValid())
		List->UnselectAll();
}

int UNaItemSlotList::GetSelectedPosition() {
	if (List.IsValid())
		return List->GetSelectedPosition();
	else return -1;
}

void UNaItemSlotList::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	List.Reset();
}