// By Sodium


#include "UI/Widgets/SNaItemSlotList.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "NaUtilityMinimal.h"
#include "Widgets/Layout/SWrapBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaItemSlotList::Construct(const FArguments& InArgs)
{

	/* Copy inputs */

	GMComponent = UNaItemStatics::GetGameModeItemSystemComponent(InArgs._WorldContext.Get());
#if NAPACK_DO_COMMON_CHECK
	if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlotList: invalid game mode component."));
		return;
	}
#endif

	ContainerPtr = InArgs.ContainerPtr.Get();
#if NAPACK_DO_COMMON_CHECK
	if (!ContainerPtr.IsValid()) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlotList: invalid item container."));
		return;
	}
#endif
	BoxSize = InArgs.BoxSize.Get();
	bHideAmountWhenOne = InArgs.bHideAmountWhenOne.Get();
	Font = InArgs.Font.Get();
	bFillDisabledToCompleteRectangle = InArgs.bFillDisabledToCompleteRectangle.Get();

	ActualLength = (!bFillDisabledToCompleteRectangle || ContainerPtr->GetSize() % RowLength == 0) ? ContainerPtr->GetSize() : (ContainerPtr->GetSize() / RowLength * RowLength) + 1;


	ChildSlot
	[
		SAssignNew(WrapBox, SWrapBox)
		.PreferredSize(BoxSize.X * (RowLength + 0.5))
	];

	int i = 0;
	Slots.Init(TSharedPtr<SNaItemSlot>, ActualLength);
	for (i = 0; i < ContainerPtr->GetSize(); ++i) {
		WrapBox->AddSlot()[
			SAssignNew(Slots[i], SNaItemSlot)
				.WorldContext(GMComponent)
				.EntryPtr(ContainerPtr->Find(i).EntryPtr)
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
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

