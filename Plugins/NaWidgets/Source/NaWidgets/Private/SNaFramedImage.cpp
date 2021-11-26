// Fill out your copyright notice in the Description page of Project Settings.


#include "SNaFramedImage.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Images/SImage.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaFramedImage::Construct(const FArguments& InArgs)
{
	
	/* Make a 3*3 grid panal with 9 aligned images */
	ChildSlot
	[
		SAssignNew(Subwidgets.GridPanel, SGridPanel)
		+ SGridPanel::Slot(1,1)
		[
			SAssignNew(Subwidgets.ImageCornerUpLeft, SImage)
		]
		+ SGridPanel::Slot(2,1)
		[
			SAssignNew(Subwidgets.ImageEdgeUp, SImage)
		]
		+ SGridPanel::Slot(3,1)
		[
			SAssignNew(Subwidgets.ImageCornerUpRight, SImage)
		]
		+ SGridPanel::Slot(1,2)
		[
			SAssignNew(Subwidgets.ImageEdgeLeft, SImage)
		]
		+ SGridPanel::Slot(2,2)
		[
			SAssignNew(Subwidgets.ImageMain, SImage)
		]
		+ SGridPanel::Slot(3,2)
		[
			SAssignNew(Subwidgets.ImageEdgeRight, SImage)
		]
		+ SGridPanel::Slot(1,3)
		[
			SAssignNew(Subwidgets.ImageCornerDownLeft, SImage)
		]
		+ SGridPanel::Slot(2,3)
		[
			SAssignNew(Subwidgets.ImageEdgeDown, SImage)
		]
		+ SGridPanel::Slot(3,3)
		[
			SAssignNew(Subwidgets.ImageCornerDownRight , SImage)
		]
	];
	
	/* Set size and image */

	FSlateBrush Brush = FSlateBrush();

	Brush.SetImageSize(FVector2D(Params.LeftEdgeWidth, Params.UpEdgeWidth));
	Brush.SetResourceObject(Params.CornerUpLeft);
	Subwidgets.ImageCornerUpLeft->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.MainWidth, Params.UpEdgeWidth));
	Brush.SetResourceObject(Params.EdgeUp);
	Subwidgets.ImageEdgeUp->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.RightEdgeWidth, Params.UpEdgeWidth));
	Brush.SetResourceObject(Params.CornerUpRight);
	Subwidgets.ImageCornerUpRight->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.LeftEdgeWidth, Params.MainHeight));
	Brush.SetResourceObject(Params.EdgeLeft);
	Subwidgets.ImageEdgeLeft->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.MainWidth, Params.MainHeight));
	Brush.SetResourceObject(Params.MainImage);
	Subwidgets.ImageMain->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.RightEdgeWidth, Params.MainHeight));
	Brush.SetResourceObject(Params.EdgeRight);
	Subwidgets.ImageEdgeRight->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.LeftEdgeWidth, Params.DownEdgeWidth));
	Brush.SetResourceObject(Params.CornerDownLeft);
	Subwidgets.ImageCornerDownLeft->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.MainWidth, Params.DownEdgeWidth));
	Brush.SetResourceObject(Params.EdgeDown);
	Subwidgets.ImageEdgeDown->SetImage(TAttribute<const FSlateBrush*>(&Brush));

	Brush.SetImageSize(FVector2D(Params.RightEdgeWidth, Params.DownEdgeWidth));
	Brush.SetResourceObject(Params.CornerDownRight);
	Subwidgets.ImageCornerDownRight->SetImage(TAttribute<const FSlateBrush*>(&Brush));

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
