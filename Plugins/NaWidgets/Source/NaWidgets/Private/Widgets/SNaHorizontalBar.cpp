// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SNaHorizontalBar.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"

float FNaBarParams::GetLengthLowEnd() {
	if (Length <= 0)
		return 0.0;
	else if (Length >= LengthLowEnd + LengthHighEnd)
		return LengthLowEnd;
	else return (Length * (LengthLowEnd) / (LengthLowEnd + LengthHighEnd));
};

float FNaBarParams::GetLengthMain() {
	return Length <= (LengthLowEnd + LengthHighEnd) ? 0.0 : (Length - LengthLowEnd - LengthHighEnd);
}

float FNaBarParams::GetLengthHighEnd() {
	if (Length <= 0)
		return 0.0;
	else if (Length >= LengthLowEnd + LengthHighEnd)
		return LengthHighEnd;
	else return (Length * (LengthHighEnd) / (LengthLowEnd + LengthHighEnd));
};


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaHorizontalBar::Construct(const FArguments& InArgs)
{
	Params.ImageBar = InArgs._ImageBar.Get();
	Params.ImageLowEnd = InArgs._ImageLeftEnd.Get();
	Params.ImageHighEnd = InArgs._ImageRightEnd.Get();
	Params.Length = InArgs._Length.Get();
	Params.LengthLowEnd = InArgs._LengthLeftEnd.Get();
	Params.LengthHighEnd = InArgs._LengthRightEnd.Get();
	Params.Width = InArgs._Width.Get();
	Params.bTiling = InArgs._Tiling.Get();

	
	BrushLeft.SetImageSize(FVector2D(Params.GetLengthLowEnd(), Params.Width));
	BrushLeft.Tiling = ESlateBrushTileType::NoTile;
	BrushLeft.SetResourceObject(Params.ImageLowEnd);

	BrushMain.SetImageSize(FVector2D(Params.GetLengthMain(), Params.Width));
	BrushMain.Tiling = InArgs._Tiling.Get() ? ESlateBrushTileType::Horizontal : ESlateBrushTileType::NoTile;
	BrushMain.SetResourceObject(Params.ImageBar);

	BrushRight.SetImageSize(FVector2D(Params.GetLengthHighEnd(), Params.Width));
	BrushRight.Tiling = ESlateBrushTileType::NoTile;
	BrushRight.SetResourceObject(Params.ImageHighEnd);

	ChildSlot
	[
		SAssignNew(Subwidgets.Box, SHorizontalBox)
		+ SHorizontalBox::Slot()[
			SAssignNew(Subwidgets.ImageLeftEnd, SImage)
				.Image(&BrushLeft)
		]
		+ SHorizontalBox::Slot()[
			SAssignNew(Subwidgets.ImageMain, SImage)
				.Image(&BrushMain)
		]
		+ SHorizontalBox::Slot()[
			SAssignNew(Subwidgets.ImageRightEnd, SImage)
				.Image(&BrushRight)
		]
	];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
