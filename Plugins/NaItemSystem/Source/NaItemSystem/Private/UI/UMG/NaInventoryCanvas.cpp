// By Sodium

#include "UI/UMG/NaInventoryCanvas.h"
#include "UI/Widgets/SNaInventoryCanvas.h"
#include "Components/NaItemInventoryComponent.h"
#include "NatriumItemSystem.h"
#include "NaUtilityMinimal.h"

#define LOCTEXT_NAMESPACE "NaItemSystem"

const FText UNaInventoryCanvas::GetPaletteCategory() {
	return LOCTEXT("NaItemSystem", "NaItemSystem");
}

TSharedRef<SWidget> UNaInventoryCanvas::RebuildWidget() {

	FNaItemSlotPublicStyle* PublicStylePtr = &PublicStyle;
	if (!IsValid(ContainerComponent)) {
		UE_LOG(LogNaItem, Display, TEXT("UNaInventoryCanvas: invalid container reference."));
		SAssignNew(CanvasWidget, SNaInventoryCanvas)
			.FromUMG(this)
			.StylePtr(PublicStylePtr)
			.SlotAmount(SlotAmount);
		return CanvasWidget.ToSharedRef();
	}
	else {
		SAssignNew(CanvasWidget, SNaInventoryCanvas)
			.Container(ContainerComponent)
			.FromUMG(this)
			.StylePtr(PublicStylePtr)
			.SlotAmount(SlotAmount);
	}

	return CanvasWidget.ToSharedRef();
}

void UNaInventoryCanvas::SetContainerComponent(UNaItemInventoryComponent* NewComponent) {
	ContainerComponent = NewComponent;
	if (CanvasWidget.IsValid())
		CanvasWidget->ResetContainer(NewComponent);
}

void UNaInventoryCanvas::Refresh() {
	if (CanvasWidget.IsValid())
		CanvasWidget->ResetAllSlots();
}

void UNaInventoryCanvas::SelectSlot(int Position) {
	if (CanvasWidget.IsValid())
		CanvasWidget->SelectSlot(Position);
}

void UNaInventoryCanvas::UnselectAll() {
	if (CanvasWidget.IsValid())
		CanvasWidget->UnselectAll();
}

int UNaInventoryCanvas::GetSelectedPosition() {
	if (CanvasWidget.IsValid())
		return CanvasWidget->GetSelectedPosition();
	else return -1;
}

void UNaInventoryCanvas::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	CanvasWidget.Reset();
}

#undef LOCTEXT_NAMESPACE
