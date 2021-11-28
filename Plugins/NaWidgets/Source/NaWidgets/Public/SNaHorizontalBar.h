// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * NaHorizontalBar is a bar widget with a custom left and right end. When stretching or setting the gauge value, the shape of ends will not change.
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
		_LeftEndLength = 4.f;
		_RightEndLength = 4.f;
		_Width = 16.f;
		_Tiling = false;
		_GaugeValue = 1.f;
	}

	/* Resource objects of bar images. */
	SLATE_ATTRIBUTE(UObject*, ImageBar)
	SLATE_ATTRIBUTE(UObject*, ImageLeftEnd)
	SLATE_ATTRIBUTE(UObject*, ImageRightEnd)
	
	/* Sizes */
	SLATE_ATTRIBUTE(float, Length)
	SLATE_ATTRIBUTE(float, LeftEndLength)
	SLATE_ATTRIBUTE(float, RightEndLength)
	SLATE_ATTRIBUTE(float, Width)

	/* Settings */
	SLATE_ATTRIBUTE(bool, Tiling)
	SLATE_ATTRIBUTE(float, GaugeValue)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
