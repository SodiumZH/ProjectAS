// By Sodium
// SNaHorizontalBar.h - Declares SNaHorizontalBar, a horizontal bar widget with fixed-size
// end caps, and its supporting types FNaBarParams and FNaHoriontalBarSubwidgets.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SNaHorizontalBar.generated.h"

/**
 * @brief Configuration parameters for an SNaHorizontalBar widget.
 *
 * Stores image references and dimensional data that fully describe how the bar
 * should be rendered.  Pass an instance of this struct to SNaHorizontalBar via
 * the SLATE_ATTRIBUTE arguments, or write to `SNaHorizontalBar::Params` after
 * construction and rebuild the widget.
 *
 * ## Bar composition (left → right)
 * @code
 * [ LowEnd ][ ——— Main image ——— ][ HighEnd ]
 *   fixed                              fixed
 * @endcode
 *
 * The LowEnd / HighEnd images always render at their specified lengths regardless
 * of the total bar length, so their shapes are preserved during resize operations.
 * Only the main image stretches (or tiles).
 */
USTRUCT(BlueprintType)
struct NAWIDGETS_API FNaBarParams {

	GENERATED_USTRUCT_BODY()

public:

	/** Main bar image, drawn between the two end caps. Stretches or tiles based on `bTiling`. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageBar;
	
	/** End-cap image for the low-value (left) side of the bar (e.g. the empty / zero end). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageLowEnd;

	/** End-cap image for the high-value (right) side of the bar (e.g. the full / max-value end). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageHighEnd;

	/** Total visual length of the bar in Slate units, including both end caps. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length;

	/** Fixed display length of the low-value (left) end cap in Slate units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LengthLowEnd;

	/** Fixed display length of the high-value (right) end cap in Slate units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LengthHighEnd;

	/** Height (vertical extent) of the bar in Slate units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width;

	/**
	 * When true the main image is tiled across the available length.
	 * When false (default) the main image is stretched to fill the space.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTiling;

	/** Default constructor — no images, 128-unit length with 4-unit end caps and 16-unit width. */
	FNaBarParams() :
		ImageBar(nullptr),
		ImageLowEnd(nullptr),
		ImageHighEnd(nullptr),
		Length(128.0),
		LengthLowEnd(4.0),
		LengthHighEnd(4.0),
		Width(16.0),
		bTiling(false)
	{};

	/**
	 * @brief Returns the actual display length of the low-value end cap.
	 *
	 * Clamps LengthLowEnd so the end caps do not exceed the total Length.
	 */
	float GetLengthLowEnd();

	/**
	 * @brief Returns the display length of the main (center) section of the bar.
	 *
	 * Calculated as: Length − GetLengthLowEnd() − GetLengthHighEnd().
	 * Returns 0 if the end caps together exceed the total Length.
	 */
	float GetLengthMain();

	/**
	 * @brief Returns the actual display length of the high-value end cap.
	 *
	 * Clamps LengthHighEnd so the end caps do not exceed the total Length.
	 */
	float GetLengthHighEnd();
};

/**
 * @brief Internal subwidget references for SNaHorizontalBar.
 *
 * Groups the three SImage widgets and their SHorizontalBox container that make up
 * the visual structure of the bar.  Exposed as a public member of SNaHorizontalBar
 * for cases where callers need direct access to the underlying Slate widgets.
 */
struct FNaHoriontalBarSubwidgets {

	/** Horizontal box container that arranges the three image segments side-by-side. */
	TSharedPtr<class SHorizontalBox> Box;

	/** SImage widget for the left (low-value) end cap. Maps to FNaBarParams::ImageLowEnd. */
	TSharedPtr<class SImage> ImageLeftEnd;

	/** SImage widget for the main (center) section. Maps to FNaBarParams::ImageBar. */
	TSharedPtr<SImage> ImageMain;

	/** SImage widget for the right (high-value) end cap. Maps to FNaBarParams::ImageHighEnd. */
	TSharedPtr<SImage> ImageRightEnd;

};


/**
 * @brief A horizontal bar widget with non-stretching fixed-size end caps.
 *
 * SNaHorizontalBar renders a three-segment horizontal bar:
 * - **Left end** (fixed width, low-value side, e.g. the "empty" cap of a health bar)
 * - **Main section** (stretches or tiles as the bar width changes)
 * - **Right end** (fixed width, high-value side, e.g. the "full" cap)
 *
 * Because the end caps have fixed sizes they retain their shape when the overall
 * bar length is adjusted at runtime, which is useful for health bars, progress bars,
 * loading indicators, and similar UI elements.
 *
 * ## Typical use cases
 * - Health / mana / stamina bars in an HUD
 * - Experience or progress bars
 * - Any bar whose end-cap artwork would look distorted if stretched
 *
 * ## Usage example
 * @code
 * TSharedRef<SNaHorizontalBar> HealthBar = SNew(SNaHorizontalBar)
 *     .ImageBar(BarTexture)
 *     .ImageLeftEnd(LeftCapTexture)
 *     .ImageRightEnd(RightCapTexture)
 *     .Length(200.f)
 *     .LengthLeftEnd(8.f)
 *     .LengthRightEnd(8.f)
 *     .Width(20.f)
 *     .Tiling(false);
 * @endcode
 *
 * @see FNaBarParams, FNaHoriontalBarSubwidgets
 */
class NAWIDGETS_API SNaHorizontalBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaHorizontalBar)
	{
		_ImageBar = nullptr;
		_ImageLeftEnd = nullptr;
		_ImageRightEnd = nullptr;
		_Length = 128.f;
		_LengthLeftEnd = 4.f;
		_LengthRightEnd = 4.f;
		_Width = 16.f;
		_Tiling = false;
	}

	/** Main bar image asset (UTexture2D, UMaterialInterface, etc.). */
	SLATE_ATTRIBUTE(UObject*, ImageBar)
	/** Left end-cap image asset. */
	SLATE_ATTRIBUTE(UObject*, ImageLeftEnd)
	/** Right end-cap image asset. */
	SLATE_ATTRIBUTE(UObject*, ImageRightEnd)
	
	/** Total length of the bar in Slate units (including both end caps). */
	SLATE_ATTRIBUTE(float, Length)
	/** Display length of the left end cap in Slate units. */
	SLATE_ATTRIBUTE(float, LengthLeftEnd)
	/** Display length of the right end cap in Slate units. */
	SLATE_ATTRIBUTE(float, LengthRightEnd)
	/** Height of the bar in Slate units. */
	SLATE_ATTRIBUTE(float, Width)

	/** When true the main image is tiled; when false it is stretched. */
	SLATE_ATTRIBUTE(bool, Tiling)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Current bar parameters.  Modify and rebuild the widget to apply changes. */
	FNaBarParams Params;

	/** Shared pointers to the internal SHorizontalBox and three SImage subwidgets. */
	FNaHoriontalBarSubwidgets Subwidgets;

	/** Slate brush for the right end-cap image. */
	FSlateBrush BrushRight = *FCoreStyle::Get().GetDefaultBrush();
	/** Slate brush for the main bar image. */
	FSlateBrush BrushMain = *FCoreStyle::Get().GetDefaultBrush();
	/** Slate brush for the left end-cap image. */
	FSlateBrush BrushLeft = *FCoreStyle::Get().GetDefaultBrush();
};
