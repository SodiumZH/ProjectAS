// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SNaHorizontalBar.generated.h"

// Parameter set of all type of bars.
USTRUCT(BlueprintType)
struct NAWIDGETS_API FNaBarParams {

	GENERATED_USTRUCT_BODY()

public:

	// Main image, excluding the end
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageBar;
	
	// Image of the side for zero value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageLowEnd;

	// Image of the side for full value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ImageHighEnd;

	// Overall length(including ends).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length;

	// Length of the left end
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LengthLowEnd;

	// Length of the right end
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LengthHighEnd;

	// Bar width
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width;

	// If true, the main image will do tiling. Or it will be stretched.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTiling;

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

	
	// Get actual length of low side end image.
	float GetLengthLowEnd();

	// Get actual length of main image (excluding ends).
	float GetLengthMain();

	// Get actual length of high side end image.
	float GetLengthHighEnd();



};

struct FNaHoriontalBarSubwidgets {

	TSharedPtr<class SHorizontalBox> Box;

	TSharedPtr<class SImage> ImageLeftEnd;

	TSharedPtr<SImage> ImageMain;

	TSharedPtr<SImage> ImageRightEnd;

};


/**
 * NaHorizontalBar is a bar widget with a custom left and right end. When stretching, the shape of ends will not change.
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

	/* Resource objects of bar images. */
	SLATE_ATTRIBUTE(UObject*, ImageBar)
	SLATE_ATTRIBUTE(UObject*, ImageLeftEnd)
	SLATE_ATTRIBUTE(UObject*, ImageRightEnd)
	
	/* Sizes */
	SLATE_ATTRIBUTE(float, Length)
	SLATE_ATTRIBUTE(float, LengthLeftEnd)
	SLATE_ATTRIBUTE(float, LengthRightEnd)
	SLATE_ATTRIBUTE(float, Width)

	/* Settings */
	SLATE_ATTRIBUTE(bool, Tiling)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FNaBarParams Params;

	FNaHoriontalBarSubwidgets Subwidgets;

	FSlateBrush BrushRight = *FCoreStyle::Get().GetDefaultBrush();
	FSlateBrush BrushMain = *FCoreStyle::Get().GetDefaultBrush();
	FSlateBrush BrushLeft = *FCoreStyle::Get().GetDefaultBrush();
};
