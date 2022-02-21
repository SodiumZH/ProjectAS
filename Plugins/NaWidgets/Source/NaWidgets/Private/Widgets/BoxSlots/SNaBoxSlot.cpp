// By Sodium


#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "Widgets/Images/SLayeredImage.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "SlateOptMacros.h"
#include "Styling/SlateBrush.h"


FNaBoxSlotParams FNaBoxSlotParams::DefaultParams = FNaBoxSlotParams();


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaBoxSlot::Construct(const FArguments& InArgs)
{

	// If _Params attribute is not set, initialize with default params
	// Initial pointer's value will be copied, then the pointer can be safely changed
	Params = (InArgs._Params.Get()) ? (*(InArgs._Params.Get())) : (FNaBoxSlotParams());

	BrushBase = TSharedPtr<FSlateBrush>(new FSlateBrush(*FCoreStyle::Get().GetDefaultBrush()));
	BrushBase->SetImageSize(Params.Size);
	BrushBase->SetResourceObject(Params.ImageBase);
	BrushBase->Tiling = TEnumAsByte<ESlateBrushTileType::Type>(ESlateBrushTileType::Type::NoTile);

	BrushFrame = TSharedPtr<FSlateBrush>(new FSlateBrush(*FCoreStyle::Get().GetDefaultBrush()));
	BrushFrame->SetImageSize(Params.Size);
	BrushFrame->SetResourceObject(Params.ImageFrame);
	BrushFrame->Tiling = TEnumAsByte<ESlateBrushTileType::Type>(ESlateBrushTileType::Type::NoTile);
	
	BrushPointed = TSharedPtr<FSlateBrush>(new FSlateBrush(*FCoreStyle::Get().GetDefaultBrush()));
	BrushPointed->SetImageSize(Params.Size);
	BrushPointed->SetResourceObject(Params.ImagePointed);
	BrushPointed->Tiling = TEnumAsByte<ESlateBrushTileType::Type>(ESlateBrushTileType::Type::NoTile);

	BrushSelected = TSharedPtr<FSlateBrush>(new FSlateBrush(*FCoreStyle::Get().GetDefaultBrush()));
	BrushSelected->SetImageSize(Params.Size);
	BrushSelected->SetResourceObject(Params.ImageSelected);
	BrushSelected->Tiling = TEnumAsByte<ESlateBrushTileType::Type>(ESlateBrushTileType::Type::NoTile);

	SubscriptFont = InArgs._SubscriptFont.Get();
	SuperscriptFont = InArgs._SuperscriptFont.Get();

	ChildSlot
	[
		SAssignNew(Overlay, SOverlay)
		+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(Images, SLayeredImage)
				.Image(TAttribute<const FSlateBrush*>(BrushBase.Get()))
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(Subscript, STextBlock)
				.Text(TAttribute<FText>(Params.SubscriptText))
				.Font(TAttribute<FSlateFontInfo>(SubscriptFont))
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SAssignNew(Superscript, STextBlock)
				.Text(TAttribute<FText>(Params.SuperscriptText))
				.Font(TAttribute<FSlateFontInfo>(SuperscriptFont))
			]
	];
	Images->AddLayer(TAttribute<const FSlateBrush*>(BrushFrame.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 1.0))));
	Images->AddLayer(TAttribute<const FSlateBrush*>(BrushPointed.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 0.0 /* Hidden by default */ ))));
	Images->AddLayer(TAttribute<const FSlateBrush*>(BrushSelected.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 0.0 /* Hidden by default */ ))));

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNaBoxSlot::SetSelected(bool NewSelectedState) {
	bIsSelected = NewSelectedState;
	Images->SetLayerColor(3, TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, NewSelectedState ? 1.0 : 0.0))));
}

void SNaBoxSlot::SetPointed(bool NewPointedState) {
	bIsPointed = NewPointedState;
	Images->SetLayerColor(2, TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, NewPointedState ? 1.0 : 0.0))));
}

void SNaBoxSlot::SetImage(ENaBoxSlotImageLayer Layer, UObject* NewImage) {
	switch (Layer) {
	case ENaBoxSlotImageLayer::BSIL_Base: {
		Params.ImageBase = NewImage;
		BrushBase->SetResourceObject(NewImage);
		Images->SetImage(TAttribute<const FSlateBrush*>(BrushBase.Get()));
		break;
	}
	case ENaBoxSlotImageLayer::BSIL_Frame: {
		Params.ImageFrame = NewImage;
		BrushFrame->SetResourceObject(NewImage);
		Images->SetLayerBrush(0, TAttribute<const FSlateBrush*>(BrushFrame.Get()));
		break;
	}
	case ENaBoxSlotImageLayer::BSIL_Pointed: {
		Params.ImagePointed = NewImage;
		BrushPointed->SetResourceObject(NewImage);
		Images->SetLayerBrush(1, TAttribute<const FSlateBrush*>(BrushFrame.Get()));
		break;
	}
	case ENaBoxSlotImageLayer::BSIL_Selected: {
		Params.ImageSelected = NewImage;
		BrushSelected->SetResourceObject(NewImage);
		Images->SetLayerBrush(2, TAttribute<const FSlateBrush*>(BrushFrame.Get()));
	}
	default: {
		checkNoEntry();
	}
	}
}

void SNaBoxSlot::Reset(const FNaBoxSlotParams & NewParams) {

	BrushBase->SetImageSize(NewParams.Size);
	BrushBase->SetResourceObject(NewParams.ImageBase);
	Images->SetImage(TAttribute<const FSlateBrush*>(BrushBase.Get()));

	BrushFrame->SetImageSize(NewParams.Size);
	BrushFrame->SetResourceObject(NewParams.ImageFrame);
	Images->SetLayerBrush(0, TAttribute<const FSlateBrush*>(BrushFrame.Get()));

	BrushPointed->SetImageSize(NewParams.Size);
	BrushPointed->SetResourceObject(NewParams.ImagePointed);
	Images->SetLayerBrush(1, TAttribute<const FSlateBrush*>(BrushFrame.Get()));

	BrushSelected->SetImageSize(NewParams.Size);
	BrushSelected->SetResourceObject(NewParams.ImageSelected);
	Images->SetLayerBrush(2, TAttribute<const FSlateBrush*>(BrushFrame.Get()));

}

void SNaBoxSlot::SetText(bool bSetSuperscript, const FText & InText) {
	if (bSetSuperscript)
		Superscript->SetText(InText);
	else
		Subscript->SetText(InText);
}

void SNaBoxSlot::SetFont(bool bSetSuperscript, const FSlateFontInfo & NewFont) {
	if (bSetSuperscript)
		Superscript->SetFont(NewFont);
	else Subscript->SetFont(NewFont);
}

FSlateFontInfo SNaBoxSlot::GetFont(bool bGetSuperscriptFont) {
	return bGetSuperscriptFont ? SuperscriptFont : SubscriptFont;
}

/* Events override */
void SNaBoxSlot::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	SWidget::OnMouseEnter(MyGeometry, MouseEvent);
	SetPointed(true);
}

void SNaBoxSlot::OnMouseLeave(const FPointerEvent& MouseEvent) {
	SWidget::OnMouseLeave(MouseEvent);
	SetPointed(false);
}