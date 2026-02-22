#include "Widgets/BoxSlots/SNaSlotList.h"
#include "NaUtilityMinimal.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaWidgets.h"

FNaSlotListDisplayInfo FNaSlotListDisplayInfo::Defaults = FNaSlotListDisplayInfo();

void FNaSlotListDisplayInfo::FixArrays() {
	if (!IsValidLength())
		return;

	if (ImageBaseArray.Num() > Length) {
		UE_LOG(LogNaWidgets, Display, TEXT("NaSlotList initiailizer: Image base array truncated from %d to Length == %d."), ImageBaseArray.Num(), Length);
		ImageBaseArray.SetNum(Length, EAllowShrinking::Yes);
	}
	else if (ImageBaseArray.Num() < Length) {
		UObject* LastElement = ImageBaseArray.Num() ? ImageBaseArray[ImageBaseArray.Num() - 1] : nullptr;
		int temp = ImageBaseArray.Num();
		int i;
		ImageBaseArray.SetNum(Length, EAllowShrinking::Yes);
		for (i = temp; i < Length; ++i)
			ImageBaseArray[i] = LastElement;
	}

	if (ImageFrameArray.Num() > Length) {
		UE_LOG(LogNaWidgets, Display, TEXT("NaSlotList initiailizer: Image frame array truncated from %d to Length == %d."), ImageFrameArray.Num(), Length);
		ImageFrameArray.SetNum(Length, EAllowShrinking::Yes);
	}
	else if (ImageFrameArray.Num() < Length) {
		UObject* LastElement = ImageFrameArray.Num() ? ImageFrameArray[ImageFrameArray.Num() - 1] : nullptr;
		int temp = ImageFrameArray.Num();
		int i;
		ImageFrameArray.SetNum(Length, EAllowShrinking::Yes);
		for (i = temp; i < Length; ++i)
			ImageFrameArray[i] = LastElement;
	}

	if (SuperscriptTextArray.Num() > Length) {
		UE_LOG(LogNaWidgets, Display, TEXT("NaSlotList display info: Superscript text array truncated from %d to Length == %d."), SuperscriptTextArray.Num(), Length);
		SuperscriptTextArray.SetNum(Length, EAllowShrinking::Yes);
	}
	else if (SuperscriptTextArray.Num() < Length) {
		FText LastElement = SuperscriptTextArray.Num() ? SuperscriptTextArray[SuperscriptTextArray.Num() - 1] : FText();
		int temp = SuperscriptTextArray.Num();
		int i;
		SuperscriptTextArray.SetNum(Length, EAllowShrinking::Yes);
		for (i = temp; i < Length; ++i) 
			SuperscriptTextArray[i] = LastElement;
	}

	if (SubscriptTextArray.Num() > Length) {
		UE_LOG(LogNaWidgets, Display, TEXT("NaSlotList display info: Subscript text array truncated from %d to Length == %d."), SubscriptTextArray.Num(), Length);
		SubscriptTextArray.SetNum(Length, EAllowShrinking::Yes);
	}
	else if (SubscriptTextArray.Num() < Length) {
		FText LastElement = SubscriptTextArray.Num() ? SubscriptTextArray[SubscriptTextArray.Num() - 1] : FText();
		int temp = SubscriptTextArray.Num();
		int i;
		SubscriptTextArray.SetNum(Length, EAllowShrinking::Yes);
		for (i = temp; i < Length; ++i)
			SubscriptTextArray[i] = LastElement;
	}

}

bool FNaSlotListDisplayInfo::IsValidLength() {
	if (Length > SLOT_LIST_MAX_LENGTH) {
		UE_LOG(LogNaWidgets, Error, TEXT("NaSlotList initializer: length overflow: %d. Limit: %d."), Length, SLOT_LIST_MAX_LENGTH);
		return false;
	}
	else if (Length < 0) {
		UE_LOG(LogNaWidgets, Error, TEXT("NaSlotList initializer: invalid length: %d"), Length);
		return false;
	}
	else if (SLOT_LIST_ENABLE_LENGTH_WARNING && (Length > SLOT_LIST_WARNING_LENGTH)) {
		UE_LOG(LogNaWidgets, Warning, TEXT("NaSlotList initializer: length may be too long. Current length: %d"), Length);
	}
	return true;
}

bool FNaSlotListDisplayInfo::AreArraysFixed(bool bDoAssert) {
	bool RetVal = (ImageBaseArray.Num() == Length) && (ImageFrameArray.Num() == Length) && (SuperscriptTextArray.Num() == Length) && (SubscriptTextArray.Num() == Length);
	if (bDoAssert && (!RetVal)) {
		UE_LOG(LogNaWidgets, Fatal, TEXT("FNaSlotListDisplayInfo error: unfixed arrays."));	// Assert
	}
	else if (!RetVal) {
		UE_LOG(LogNaWidgets, Error, TEXT("FNaSlotListDisplayInfo error: unfixed arrays.")); // Only print error
	}
	return RetVal;
}

void FNaSlotListDisplayInfo::GetSlotParams(FNaBoxSlotParams & Out, int Position, bool bNeedInit) {
	
#if NAPACK_DO_VERBOSE_CHECK
	if (Position >= Length || Position < 0) {
		UE_LOG(LogNaWidgets, Warning, TEXT("FNaSlotListDisplayInfo::GetSlotParams: invalid position."));
		return;
	}
	AreArraysFixed(true);	// Assert arrays fixed
#endif
	Out.ImageBase = ImageBaseArray[Position];
	Out.ImageFrame = ImageFrameArray[Position];
	Out.SuperscriptText = SuperscriptTextArray[Position];
	Out.SubscriptText = SubscriptTextArray[Position];
	if (bNeedInit) {
		Out.Size = BoxSize;
		Out.ImagePointed = ImagePointed;
		Out.ImageSelected = ImageSelected;
	}
	return;
}

TSharedPtr<FNaBoxSlotParams> FNaSlotListDisplayInfo::GetSlotParams(int Position) {
	if (Position >= Length || Position < 0) {
		UE_LOG(LogNaWidgets, Warning, TEXT("FNaSlotListDisplayInfo::GetSlotParams: invalid position."));
		return TSharedPtr<FNaBoxSlotParams>(nullptr);
	}
	
	AreArraysFixed(true);	// Assert arrays fixed

	TSharedPtr<FNaBoxSlotParams> Out = TSharedPtr<FNaBoxSlotParams>(new FNaBoxSlotParams());
	
	Out->ImageBase = ImageBaseArray[Position];
	Out->ImageFrame = ImageFrameArray[Position];
	Out->SuperscriptText = SuperscriptTextArray[Position];
	Out->SubscriptText = SubscriptTextArray[Position];
	Out->Size = BoxSize;
	Out->ImagePointed = ImagePointed;
	Out->ImageSelected = ImageSelected;
	
	return Out;
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaSlotList::Construct(const FArguments& InArgs)
{
	
	/* Copy init values */
	RowLength = InArgs._RowLength.Get();
	DisplayInfo = InArgs._DisplayInfo.Get() ? (*(InArgs._DisplayInfo.Get())) : FNaSlotListDisplayInfo::Defaults;
	check(RowLength > 0);
	SuperscriptFont = InArgs._SuperscriptFont.Get();
	SubscriptFont = InArgs._SubscriptFont.Get();

	/* Init slot array */
	BoxSlots.Init(TSharedPtr<SNaBoxSlot>(nullptr), DisplayInfo.Length);

	// Create empty wrap box first
	ChildSlot
	[
		SAssignNew(WrapBox, SWrapBox)
		.PreferredSize(DisplayInfo.BoxSize.X * (RowLength + 0.5))
	];
	
	// Then fill with slots
	int i = 0;
	for (i = 0; i < DisplayInfo.Length; ++i) {
		DisplayInfo.GetSlotParams(TempSlotParams, i, i == 0);	// Fetch slot params from display info
		// Create a child slot
		WrapBox->AddSlot()
			[
				SAssignNew(BoxSlots[i], SNaBoxSlot)
				.Params(TAttribute<const FNaBoxSlotParams *>(&TempSlotParams))
				.SuperscriptFont(SuperscriptFont)
				.SubscriptFont(SubscriptFont)
			];
	}	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SNaSlotList::IsSlotArrayLengthRight() {
	if (BoxSlots.Num() != DisplayInfo.Length) {
		UE_LOG(LogNaWidgets, Error, TEXT("SNaSlotArray error: child box array length doesn't match length in params."));
		return false;
	}
	else return true;
}

void SNaSlotList::SetSlot(int Position, const FNaBoxSlotParams & Params) {

#if NAPACK_DO_VERBOSE_CHECK

	if (!IsSlotArrayLengthRight())
		return;

	if (Position >= DisplayInfo.Length || Position < 0) {
		UE_LOG(LogNaWidgets, Warning, TEXT("SNaSlotList::SetSlot: invalid position."));
		return;
	}

#endif

	// Set slot
	BoxSlots[Position]->SetImage(ENaBoxSlotImageLayer::BSIL_Base, Params.ImageBase);
	BoxSlots[Position]->SetImage(ENaBoxSlotImageLayer::BSIL_Frame, Params.ImageFrame);
	BoxSlots[Position]->SetText(true, Params.SuperscriptText);
	BoxSlots[Position]->SetText(false, Params.SubscriptText);

	// Update param struct
	DisplayInfo.ImageBaseArray[Position] = Params.ImageBase;
	DisplayInfo.ImageFrameArray[Position] = Params.ImageFrame;
	DisplayInfo.SuperscriptTextArray[Position] = Params.SuperscriptText;
	DisplayInfo.SubscriptTextArray[Position] = Params.SubscriptText;

}

void SNaSlotList::SetSlotBaseImage(int Position, UObject* NewImage) {

#if NAPACK_DO_VERBOSE_CHECK
	if (!IsSlotArrayLengthRight())return;

	if (Position >= DisplayInfo.Length || Position < 0) {
		UE_LOG(LogNaWidgets, Warning, TEXT("SNaSlotList::SetSlot: invalid position."));
		return;
	}

#endif

	BoxSlots[Position]->SetImage(ENaBoxSlotImageLayer::BSIL_Base, NewImage);
	DisplayInfo.ImageBaseArray[Position] = NewImage;
}

void SNaSlotList::SetSlotFrameImage(int Position, UObject* NewImage) {

#if NAPACK_DO_VERBOSE_CHECK

	if (!IsSlotArrayLengthRight())return;

	if (Position >= DisplayInfo.Length || Position < 0) {
		UE_LOG(LogNaWidgets, Warning, TEXT("SNaSlotList::SetSlot: invalid position."));
		return;
	}

#endif

	BoxSlots[Position]->SetImage(ENaBoxSlotImageLayer::BSIL_Frame, NewImage);
	DisplayInfo.ImageFrameArray[Position] = NewImage;
}

void SNaSlotList::SetSlotText(int Position, const FText & NewText, bool bSetSuperscript) {

#if NAPACK_DO_VERBOSE_CHECK

	if (!IsSlotArrayLengthRight())return;

	if (Position >= DisplayInfo.Length || Position < 0) {
		UE_LOG(LogNaWidgets, Warning, TEXT("SNaSlotList::SetSlot: invalid position."));
		return;
	}

#endif

	BoxSlots[Position]->SetText(bSetSuperscript, NewText);

	if (bSetSuperscript)
		DisplayInfo.SuperscriptTextArray[Position] = NewText;
	else
		DisplayInfo.SubscriptTextArray[Position] = NewText;

}




void SNaSlotList::SetSlotPointedImage(UObject* NewImage) {

#if NAPACK_DO_VERBOSE_CHECK

	if (!IsSlotArrayLengthRight())return;

#endif

	for (auto & Slot : BoxSlots) {
		Slot->SetImage(ENaBoxSlotImageLayer::BSIL_Pointed, NewImage);
	}
	DisplayInfo.ImagePointed = NewImage;
}

void SNaSlotList::SetSlotSelectedImage(UObject* NewImage) {

#if NAPACK_DO_VERBOSE_CHECK

	if (!IsSlotArrayLengthRight())return;

#endif

	for (auto & Slot : BoxSlots) {
		Slot->SetImage(ENaBoxSlotImageLayer::BSIL_Selected, NewImage);
	}
	DisplayInfo.ImageSelected = NewImage;
}

void SNaSlotList::SetFont(bool bSetSuperscript, const FSlateFontInfo & NewFont) {
	
#if NAPACK_DO_VERBOSE_CHECK

	if (!IsSlotArrayLengthRight())return;

#endif

	for (auto & Slot : BoxSlots) {
		Slot->SetFont(bSetSuperscript, NewFont);
	}
	if (bSetSuperscript)
		SuperscriptFont = NewFont;
	else
		SubscriptFont = NewFont;

}