// By Sodium

#include "Widgets/WindowWidgets/SNaBorderedWindow.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SNullWidget.h"
#include "Styling/CoreStyle.h"

static FSlateBrush MakeBorderedWindowBrush(UObject* ResourceObj, FVector2D Size)
{
	FSlateBrush Brush = *FCoreStyle::Get().GetDefaultBrush();
	Brush.SetResourceObject(ResourceObj);
	Brush.SetImageSize(Size);
	Brush.Tiling = ESlateBrushTileType::NoTile;
	return Brush;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaBorderedWindow::Construct(const FArguments& InArgs)
{
	if (InArgs._Params.Get())
		Params = *InArgs._Params.Get();
	MinBodySize = InArgs._MinBodySize.Get();
	MaxBodySize = InArgs._MaxBodySize.Get();
	ContentWidget = InArgs._Content.Get();

	// Initialize brushes from params
	BrushTopLeft     = MakeBorderedWindowBrush(Params.ImageTopLeft,     FVector2D(Params.BorderLeft,  Params.BorderTop));
	BrushTop         = MakeBorderedWindowBrush(Params.ImageTop,         FVector2D(Params.BodySize.X,  Params.BorderTop));
	BrushTopRight    = MakeBorderedWindowBrush(Params.ImageTopRight,    FVector2D(Params.BorderRight, Params.BorderTop));
	BrushLeft        = MakeBorderedWindowBrush(Params.ImageLeft,        FVector2D(Params.BorderLeft,  Params.BodySize.Y));
	BrushCenter      = MakeBorderedWindowBrush(Params.ImageCenter,      Params.BodySize);
	BrushRight       = MakeBorderedWindowBrush(Params.ImageRight,       FVector2D(Params.BorderRight, Params.BodySize.Y));
	BrushBottomLeft  = MakeBorderedWindowBrush(Params.ImageBottomLeft,  FVector2D(Params.BorderLeft,  Params.BorderBottom));
	BrushBottom      = MakeBorderedWindowBrush(Params.ImageBottom,      FVector2D(Params.BodySize.X,  Params.BorderBottom));
	BrushBottomRight = MakeBorderedWindowBrush(Params.ImageBottomRight, FVector2D(Params.BorderRight, Params.BorderBottom));

	// Create image sub-widgets
	SAssignNew(ImgTopLeft,     SImage).Image(&BrushTopLeft);
	SAssignNew(ImgTop,         SImage).Image(&BrushTop);
	SAssignNew(ImgTopRight,    SImage).Image(&BrushTopRight);
	SAssignNew(ImgLeft,        SImage).Image(&BrushLeft);
	SAssignNew(ImgCenter,      SImage).Image(&BrushCenter);
	SAssignNew(ImgRight,       SImage).Image(&BrushRight);
	SAssignNew(ImgBottomLeft,  SImage).Image(&BrushBottomLeft);
	SAssignNew(ImgBottom,      SImage).Image(&BrushBottom);
	SAssignNew(ImgBottomRight, SImage).Image(&BrushBottomRight);

	RebuildLayout();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNaBorderedWindow::RebuildLayout()
{
	// Update brush sizes to match current params
	BrushTopLeft.SetImageSize(FVector2D(Params.BorderLeft,  Params.BorderTop));
	BrushTop.SetImageSize(FVector2D(Params.BodySize.X,  Params.BorderTop));
	BrushTopRight.SetImageSize(FVector2D(Params.BorderRight, Params.BorderTop));
	BrushLeft.SetImageSize(FVector2D(Params.BorderLeft,  Params.BodySize.Y));
	BrushCenter.SetImageSize(Params.BodySize);
	BrushRight.SetImageSize(FVector2D(Params.BorderRight, Params.BodySize.Y));
	BrushBottomLeft.SetImageSize(FVector2D(Params.BorderLeft,  Params.BorderBottom));
	BrushBottom.SetImageSize(FVector2D(Params.BodySize.X,  Params.BorderBottom));
	BrushBottomRight.SetImageSize(FVector2D(Params.BorderRight, Params.BorderBottom));

	const FVector2D TotalSize(
		Params.BorderLeft + Params.BodySize.X + Params.BorderRight,
		Params.BorderTop  + Params.BodySize.Y + Params.BorderBottom
	);

	// Slot positions
	const FVector2D PosTopLeft    (0.f,                                    0.f);
	const FVector2D PosTop        (Params.BorderLeft,                      0.f);
	const FVector2D PosTopRight   (Params.BorderLeft + Params.BodySize.X,  0.f);
	const FVector2D PosLeft       (0.f,                                    Params.BorderTop);
	const FVector2D PosCenter     (Params.BorderLeft,                      Params.BorderTop);
	const FVector2D PosRight      (Params.BorderLeft + Params.BodySize.X,  Params.BorderTop);
	const FVector2D PosBottomLeft (0.f,                                    Params.BorderTop + Params.BodySize.Y);
	const FVector2D PosBottom     (Params.BorderLeft,                      Params.BorderTop + Params.BodySize.Y);
	const FVector2D PosBottomRight(Params.BorderLeft + Params.BodySize.X,  Params.BorderTop + Params.BodySize.Y);

	// Slot sizes
	const FVector2D SzCornerTL(Params.BorderLeft,  Params.BorderTop);
	const FVector2D SzTop     (Params.BodySize.X,  Params.BorderTop);
	const FVector2D SzCornerTR(Params.BorderRight, Params.BorderTop);
	const FVector2D SzLeft    (Params.BorderLeft,  Params.BodySize.Y);
	const FVector2D SzCenter  (Params.BodySize);
	const FVector2D SzRight   (Params.BorderRight, Params.BodySize.Y);
	const FVector2D SzCornerBL(Params.BorderLeft,  Params.BorderBottom);
	const FVector2D SzBottom  (Params.BodySize.X,  Params.BorderBottom);
	const FVector2D SzCornerBR(Params.BorderRight, Params.BorderBottom);

	// Optional content widget placed over the center area
	const TSharedRef<SWidget> CenterContent = ContentWidget.IsValid()
		? ContentWidget.ToSharedRef()
		: SNullWidget::NullWidget;

	// Rebuild canvas with updated absolute positions/sizes
	SAssignNew(Canvas, SCanvas)
		+ SCanvas::Slot().Position(PosTopLeft).Size(SzCornerTL)    [ImgTopLeft.ToSharedRef()]
		+ SCanvas::Slot().Position(PosTop).Size(SzTop)             [ImgTop.ToSharedRef()]
		+ SCanvas::Slot().Position(PosTopRight).Size(SzCornerTR)   [ImgTopRight.ToSharedRef()]
		+ SCanvas::Slot().Position(PosLeft).Size(SzLeft)           [ImgLeft.ToSharedRef()]
		+ SCanvas::Slot().Position(PosCenter).Size(SzCenter)       [ImgCenter.ToSharedRef()]
		+ SCanvas::Slot().Position(PosRight).Size(SzRight)         [ImgRight.ToSharedRef()]
		+ SCanvas::Slot().Position(PosBottomLeft).Size(SzCornerBL) [ImgBottomLeft.ToSharedRef()]
		+ SCanvas::Slot().Position(PosBottom).Size(SzBottom)       [ImgBottom.ToSharedRef()]
		+ SCanvas::Slot().Position(PosBottomRight).Size(SzCornerBR)[ImgBottomRight.ToSharedRef()]
		+ SCanvas::Slot().Position(PosCenter).Size(SzCenter)       [CenterContent];

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(TotalSize.X)
		.HeightOverride(TotalSize.Y)
		[
			Canvas.ToSharedRef()
		]
	];
}

FVector2D SNaBorderedWindow::ClampBodySize(FVector2D Size) const
{
	return FVector2D(
		FMath::Clamp(Size.X, MinBodySize.X, MaxBodySize.X),
		FMath::Clamp(Size.Y, MinBodySize.Y, MaxBodySize.Y)
	);
}

void SNaBorderedWindow::SetBodySize(FVector2D NewSize)
{
	Params.BodySize = ClampBodySize(NewSize);
	RebuildLayout();
}

void SNaBorderedWindow::SetBorderWidths(float Top, float Bottom, float Left, float Right)
{
	Params.BorderTop    = Top;
	Params.BorderBottom = Bottom;
	Params.BorderLeft   = Left;
	Params.BorderRight  = Right;
	RebuildLayout();
}

void SNaBorderedWindow::UpdateImages(const FNaBorderedWindowParams& NewParams)
{
	Params = NewParams;
	BrushTopLeft.SetResourceObject(Params.ImageTopLeft);
	BrushTop.SetResourceObject(Params.ImageTop);
	BrushTopRight.SetResourceObject(Params.ImageTopRight);
	BrushLeft.SetResourceObject(Params.ImageLeft);
	BrushCenter.SetResourceObject(Params.ImageCenter);
	BrushRight.SetResourceObject(Params.ImageRight);
	BrushBottomLeft.SetResourceObject(Params.ImageBottomLeft);
	BrushBottom.SetResourceObject(Params.ImageBottom);
	BrushBottomRight.SetResourceObject(Params.ImageBottomRight);
	RebuildLayout();
}

FReply SNaBorderedWindow::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		const FVector2D LocalPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		const EWindowRegion Region = GetRegionAtPosition(MyGeometry, LocalPos);

		if (Region == EWindowRegion::TopBorder)
		{
			bIsDragging = true;
			DragStartPosition = MouseEvent.GetScreenSpacePosition();
			DragStartRenderOffset = CurrentRenderOffset;
			return FReply::Handled().CaptureMouse(SharedThis(this));
		}
		else if (Region == EWindowRegion::BottomRightCorner)
		{
			bIsResizing = true;
			DragStartPosition = LocalPos;
			DragStartBodySize = Params.BodySize;
			return FReply::Handled().CaptureMouse(SharedThis(this));
		}
	}

	return FReply::Unhandled();
}

FReply SNaBorderedWindow::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (bIsDragging || bIsResizing)
		{
			bIsDragging = false;
			bIsResizing = false;
			return FReply::Handled().ReleaseMouseCapture();
		}
	}

	return FReply::Unhandled();
}

FReply SNaBorderedWindow::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsResizing)
	{
		const FVector2D CurrentPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		const FVector2D Delta = CurrentPos - DragStartPosition;

		FVector2D NewBodySize = DragStartBodySize + Delta;
		Params.BodySize = ClampBodySize(NewBodySize);
		RebuildLayout();
		return FReply::Handled();
	}
	else if (bIsDragging)
	{
		const FVector2D Delta = MouseEvent.GetScreenSpacePosition() - DragStartPosition;
		CurrentRenderOffset = DragStartRenderOffset + Delta;
		SetRenderTransform(FSlateRenderTransform(CurrentRenderOffset));
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

SNaBorderedWindow::EWindowRegion SNaBorderedWindow::GetRegionAtPosition(
	const FGeometry& MyGeometry, const FVector2D& LocalPosition) const
{
	// Bottom-right corner
	if (LocalPosition.X >= Params.BorderLeft + Params.BodySize.X &&
		LocalPosition.Y >= Params.BorderTop  + Params.BodySize.Y)
	{
		return EWindowRegion::BottomRightCorner;
	}

	// Top border (excluding corners)
	if (LocalPosition.Y < Params.BorderTop &&
		LocalPosition.X >= Params.BorderLeft &&
		LocalPosition.X <  Params.BorderLeft + Params.BodySize.X)
	{
		return EWindowRegion::TopBorder;
	}

	// Center
	if (LocalPosition.X >= Params.BorderLeft &&
		LocalPosition.X <  Params.BorderLeft + Params.BodySize.X &&
		LocalPosition.Y >= Params.BorderTop  &&
		LocalPosition.Y <  Params.BorderTop  + Params.BodySize.Y)
	{
		return EWindowRegion::Center;
	}

	return EWindowRegion::None;
}
