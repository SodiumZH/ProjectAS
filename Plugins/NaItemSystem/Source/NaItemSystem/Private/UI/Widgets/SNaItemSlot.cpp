// By Sodium


#include "UI/Widgets/SNaItemSlot.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "NaUtilityMinimal.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaItemSlot::Construct(const FArguments& InArgs)
{

	GMComponent = UNaItemStatics::GetGameModeItemSystemComponent(InArgs._WorldContext.Get());
	
#if NAPACK_DO_COMMON_CHECK
	if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Error, TEXT("NaItemSlot: invalid game mode component."));
		return;
	}
#endif

	/*  */
	ItemEntry = InArgs._EntryPtr.Get().IsValid() ? (*(InArgs._EntryPtr.Get())) : FNaItemEntry();
	bIsDisabled = InArgs._bIsDisabled.Get();
	bIsEmpty = InArgs._bIsEmpty.Get();
	bHideAmountWhenOne = InArgs._bHideAmountWhenOne.Get();

	FNaBoxSlotParams InParams = FNaBoxSlotParams();
	InParams.Size = InArgs._Size.Get();
	InParams.ImageFrame = bIsDisabled ? GMComponent->DefaultSlotSettings.IconBorderDisabled : GMComponent->DefaultSlotSettings.IconBorder;
	InParams.ImageSelected = (bIsDisabled && !GMComponent->DefaultSlotSettings.bCanSelectDisabled) ? nullptr : GMComponent->DefaultSlotSettings.IconSelected;
	InParams.ImagePointed = (bIsDisabled && !GMComponent->DefaultSlotSettings.bCanPointAtDisabled) ? nullptr : GMComponent->DefaultSlotSettings.IconPointed;
	InParams.ImageBase = bIsDisabled ? GMComponent->DefaultSlotSettings.IconDisabled : (bIsEmpty ? GMComponent->DefaultSlotSettings.IconEmpty : UNaItemDataStatics::GetItemDisplayDataFromID(InArgs._WorldContext.Get(), ItemEntry.TypeDescriptor.ItemTypeID).BrushImage);
	InParams.SuperscriptText = FText();
	InParams.SubscriptText = (bHideAmountWhenOne && (ItemEntry.Amount == 1)) ? FText() : FText::FromString(FString::FromInt(ItemEntry.Amount));

	ChildSlot
	[
		SAssignNew(BoxSlot, SNaBoxSlot)
		.Params(&InParams)
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
