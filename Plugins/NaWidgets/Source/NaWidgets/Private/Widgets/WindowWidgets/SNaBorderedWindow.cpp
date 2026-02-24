// By Sodium

#include "Widgets/WindowWidgets/SNaBorderedWindow.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SNullWidget.h"
#include "Styling/CoreStyle.h"
#include "Framework/Application/SlateApplication.h"

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

	// All positions relative to center's top-left (0,0)
	const FVector2D PosTopLeft    (-Params.BorderLeft,           -Params.BorderTop);
	const FVector2D PosTop        (0.f,                          -Params.BorderTop);
	const FVector2D PosTopRight   (Params.BodySize.X,            -Params.BorderTop);
	const FVector2D PosLeft       (-Params.BorderLeft,           0.f);
	const FVector2D PosCenter     (0.f,                          0.f);
	const FVector2D PosRight      (Params.BodySize.X,            0.f);
	const FVector2D PosBottomLeft (-Params.BorderLeft,           Params.BodySize.Y);
	const FVector2D PosBottom     (0.f,                          Params.BodySize.Y);
	const FVector2D PosBottomRight(Params.BodySize.X,            Params.BodySize.Y);

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

	// Create transparent overlay buttons for drag and resize
	SAssignNew(DragButton, SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnPressed(this, &SNaBorderedWindow::OnDragButtonPressed)
		.OnReleased(this, &SNaBorderedWindow::OnDragButtonReleased)
		.Cursor(EMouseCursor::GrabHand)
		[
			SNullWidget::NullWidget
		];

	SAssignNew(ResizeButton, SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnPressed(this, &SNaBorderedWindow::OnResizeButtonPressed)
		.OnReleased(this, &SNaBorderedWindow::OnResizeButtonReleased)
		.Cursor(EMouseCursor::ResizeSouthEast)
		[
			SNullWidget::NullWidget
		];

	// Optional content widget placed over the center area
	const TSharedRef<SWidget> CenterContent = ContentWidget.IsValid()
		? ContentWidget.ToSharedRef()
		: SNullWidget::NullWidget;

	// Rebuild inner canvas with updated positions/sizes (center as origin)
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
		+ SCanvas::Slot().Position(PosCenter).Size(SzCenter)       [CenterContent]
		+ SCanvas::Slot().Position(PosTop).Size(SzTop)             [DragButton.ToSharedRef()]
		+ SCanvas::Slot().Position(PosBottomRight).Size(SzCornerBR)[ResizeButton.ToSharedRef()];

	// Outer canvas: WindowPosition represents the center's top-left; offset by borders to get the whole window's top-left
	SAssignNew(OuterCanvas, SCanvas)
		+ SCanvas::Slot()
		  .Position(TAttribute<FVector2D>::CreateSP(this, &SNaBorderedWindow::GetOuterCanvasPosition))
		  .Size(TotalSize)
		  [
			SNew(SBox)
			.WidthOverride(TotalSize.X)
			.HeightOverride(TotalSize.Y)
			[
				Canvas.ToSharedRef()
			]
		  ];

	ChildSlot
	[
		OuterCanvas.ToSharedRef()
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

void SNaBorderedWindow::OnDragButtonPressed()
{
	bIsDragging = true;
	DragStartPosition = FSlateApplication::Get().GetCursorPos();
	DragStartWindowPosition = WindowPosition;
}

void SNaBorderedWindow::OnDragButtonReleased()
{
	bIsDragging = false;
}

void SNaBorderedWindow::OnResizeButtonPressed()
{
	bIsResizing = true;
	DragStartPosition = FSlateApplication::Get().GetCursorPos();
	DragStartBodySize = Params.BodySize;
}

void SNaBorderedWindow::OnResizeButtonReleased()
{
	bIsResizing = false;
}

void SNaBorderedWindow::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (bIsDragging)
	{
		const FVector2D CurrentCursorPos = FSlateApplication::Get().GetCursorPos();
		const FVector2D Delta = CurrentCursorPos - DragStartPosition;
		WindowPosition = DragStartWindowPosition + Delta;
		OuterCanvas->Invalidate(EInvalidateWidgetReason::Layout);
	}
	else if (bIsResizing)
	{
		const FVector2D CurrentCursorPos = FSlateApplication::Get().GetCursorPos();
		const FVector2D Delta = CurrentCursorPos - DragStartPosition;

		FVector2D NewBodySize = DragStartBodySize + Delta;
		Params.BodySize = ClampBodySize(NewBodySize);
		RebuildLayout();
	}
}
