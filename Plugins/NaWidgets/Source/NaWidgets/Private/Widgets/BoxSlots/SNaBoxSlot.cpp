// By Sodium


#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "Widgets/Images/SLayeredImage.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "SlateOptMacros.h"
#include "Styling/SlateBrush.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaBoxSlot::Construct(const FArguments& InArgs)
{

	Params = InArgs._Params.Get();

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
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SAssignNew(Superscript, STextBlock)
				.Text(TAttribute<FText>(Params.SuperscriptText))
			]
	];
	//Images->AddLayer(TAttribute<const FSlateBrush*>(BrushBase.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 1.0))));
	Images->AddLayer(TAttribute<const FSlateBrush*>(BrushFrame.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 1.0))));
	Images->AddLayer(TAttribute<const FSlateBrush*>(BrushPointed.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 0.0 /* Hidden by default */ ))));
	Images->AddLayer(TAttribute<const FSlateBrush*>(BrushSelected.Get()), TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, 0.0 /* Hidden by default */ ))));

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNaBoxSlot::SetSelected(bool NewSelectedState) {
	bIsSelected = NewSelectedState;
	Images->SetLayerColor(2, TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, NewSelectedState ? 1.0 : 0.0))));
}

void SNaBoxSlot::SetPointed(bool NewPointedState) {
	bIsPointed = NewPointedState;
	Images->SetLayerColor(1, TAttribute<FSlateColor>(FSlateColor(FLinearColor(1.0, 1.0, 1.0, NewPointedState ? 1.0 : 0.0))));
}
