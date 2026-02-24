// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SCanvas.h"
#include "Styling/SlateBrush.h"
#include "SNaBorderedWindow.generated.h"

/** Configuration struct for all 9 image parts and size parameters of SNaBorderedWindow. */
USTRUCT(BlueprintType)
struct NAWIDGETS_API FNaBorderedWindowParams
{
	GENERATED_BODY()

	/** Main body size (center rectangle). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D BodySize = FVector2D(200.f, 150.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BorderTop = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BorderBottom = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BorderLeft = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BorderRight = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageCenter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageTop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageBottom = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageTopLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageTopRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageBottomLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageBottomRight = nullptr;
};

/**
 * SNaBorderedWindow is a bordered window widget composed of 9 image parts.
 * Supports dragging (top border) and resizing (bottom-right corner).
 */
class NAWIDGETS_API SNaBorderedWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaBorderedWindow)
	{
		_Params = nullptr;
		_MinBodySize = FVector2D(50.f, 50.f);
		_MaxBodySize = FVector2D(1000.f, 1000.f);
	}

	/** Parameters for images and initial sizes. Only used during construction. */
	SLATE_ATTRIBUTE(const FNaBorderedWindowParams*, Params)
	SLATE_ATTRIBUTE(FVector2D, MinBodySize)
	SLATE_ATTRIBUTE(FVector2D, MaxBodySize)
	/** Optional content widget placed over the center body area. */
	SLATE_ATTRIBUTE(TSharedPtr<SWidget>, Content)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/** Resize the body area, clamped to MinBodySize/MaxBodySize. */
	void SetBodySize(FVector2D NewSize);

	/** Update all four border widths and rebuild layout. */
	void SetBorderWidths(float Top, float Bottom, float Left, float Right);

	/** Replace all image resources and rebuild layout. */
	void UpdateImages(const FNaBorderedWindowParams& NewParams);

	/** Returns the current screen-space position of the window (updated while dragging). */
	FVector2D GetWindowPosition() const { return WindowPosition; }

protected:
	TSharedPtr<SCanvas> OuterCanvas;
	TSharedPtr<SCanvas> Canvas;

	/* The 9 image sub-widgets. */
	TSharedPtr<SImage> ImgCenter;
	TSharedPtr<SImage> ImgTop;
	TSharedPtr<SImage> ImgBottom;
	TSharedPtr<SImage> ImgLeft;
	TSharedPtr<SImage> ImgRight;
	TSharedPtr<SImage> ImgTopLeft;
	TSharedPtr<SImage> ImgTopRight;
	TSharedPtr<SImage> ImgBottomLeft;
	TSharedPtr<SImage> ImgBottomRight;

	/* Brushes for the 9 image parts. */
	FSlateBrush BrushCenter;
	FSlateBrush BrushTop;
	FSlateBrush BrushBottom;
	FSlateBrush BrushLeft;
	FSlateBrush BrushRight;
	FSlateBrush BrushTopLeft;
	FSlateBrush BrushTopRight;
	FSlateBrush BrushBottomLeft;
	FSlateBrush BrushBottomRight;

	/** Transparent overlay buttons for drag and resize interactions. */
	TSharedPtr<SButton> DragButton;
	TSharedPtr<SButton> ResizeButton;

	FNaBorderedWindowParams Params;
	FVector2D MinBodySize;
	FVector2D MaxBodySize;

	TSharedPtr<SWidget> ContentWidget;

	bool bIsDragging = false;
	bool bIsResizing = false;
	FVector2D DragStartPosition;
	FVector2D DragStartBodySize;

	/** Current layout position of the center area within the outer canvas. */
	FVector2D WindowPosition = FVector2D::ZeroVector;
	/** Snapshot of WindowPosition taken when a drag begins. */
	FVector2D DragStartWindowPosition = FVector2D::ZeroVector;

	/** Returns the position of the outer canvas slot (WindowPosition offset by borders). */
	FVector2D GetOuterCanvasPosition() const { return WindowPosition + FVector2D(-Params.BorderLeft, -Params.BorderTop); }

	/** Rebuild the canvas layout after any size or image change. */
	void RebuildLayout();

	/** Clamp a candidate body size to [MinBodySize, MaxBodySize]. */
	FVector2D ClampBodySize(FVector2D Size) const;

	void OnDragButtonPressed();
	void OnDragButtonReleased();
	void OnResizeButtonPressed();
	void OnResizeButtonReleased();

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
};
