// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Images/SImage.h"
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
 * Supports resizing via the bottom-right corner. Dragging is not supported.
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

protected:
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

	FNaBorderedWindowParams Params;
	FVector2D MinBodySize;
	FVector2D MaxBodySize;

	TSharedPtr<SWidget> ContentWidget;

	bool bIsResizing = false;
	FVector2D DragStartPosition;
	FVector2D DragStartBodySize;

	/** Rebuild the canvas layout after any size or image change. */
	void RebuildLayout();

	/** Clamp a candidate body size to [MinBodySize, MaxBodySize]. */
	FVector2D ClampBodySize(FVector2D Size) const;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	enum class EWindowRegion
	{
		None,
		BottomRightCorner,
		Center
	};

	EWindowRegion GetRegionAtPosition(const FGeometry& MyGeometry, const FVector2D& LocalPosition) const;
};
