// By Sodium


#include "UI/Widgets/SNaItemSlotList.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "NaUtilityMinimal.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Components/NaItemContainerComponent.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaItemSlotList::Construct(const FArguments& InArgs)
{

	Container = InArgs._Container.Get();

	// When container is invalid, this widget is invalid
	if (!IsValid(Container)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlotList: invalid item container."));
		bIsInvalid = true;
	}

	/* Get context and container information */

	GMComponent = UNaItemStatics::GetGameModeItemSystemComponent(InArgs._Container.Get());

	// When game mode component is invalid, this widget is invalid
	if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlotList: invalid game mode component."));
		bIsInvalid = true;
	}



	// For valid case
	if (!bIsInvalid) {

		/* Copy inputs */
		BoxSize = InArgs._BoxSize.Get();
		bHideAmountWhenOne = InArgs._bHideAmountWhenOne.Get();
		Font = InArgs._Font.Get();
		bFillDisabledToCompleteRectangle = InArgs._bFillDisabledToCompleteRectangle.Get();
		RowLength = InArgs._RowLength.Get();

		ActualLength = (!bFillDisabledToCompleteRectangle || Container->Container.GetSize() % RowLength == 0) ? Container->Container.GetSize() : (Container->Container.GetSize() / RowLength * RowLength) + 1;


		ChildSlot
			[
				SAssignNew(WrapBox, SWrapBox)
				.PreferredSize(BoxSize.X * (RowLength + 0.5))
			];

		int i = 0;
		Slots.Init(TSharedPtr<SNaItemSlot>(nullptr), ActualLength);
		for (i = 0; i < Container->Container.GetSize(); ++i) {
			WrapBox->AddSlot()[
				SAssignNew(Slots[i], SNaItemSlot)
					.WorldContext(GMComponent)
					.EntryPtr(Container->Container.Find(i).EntryPtr)
					.Size(BoxSize)
					.bHideAmountWhenOne(bHideAmountWhenOne)
					.Font(Font)
			];
		}
		for (1; i < ActualLength; ++i) {
			WrapBox->AddSlot()[
				SAssignNew(Slots[i], SNaItemSlot)
					.WorldContext(GMComponent)
					.Size(BoxSize)
					.bHideAmountWhenOne(bHideAmountWhenOne)
					.Font(Font)
					.bIsDisabled(true)
			];
		}
	}

	return;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

