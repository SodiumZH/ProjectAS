// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SNaFramedImage.generated.h"

class SGridPanel;
class SImage;


/* Struct to construct framed image */
USTRUCT(BlueprintType)
struct NAWIDGETS_API FNaFramedImageParameters
{

	GENERATED_USTRUCT_BODY()

public:

	FNaFramedImageParameters() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* MainImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* EdgeUp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* EdgeDown = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* EdgeLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* EdgeRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* CornerUpLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* CornerUpRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* CornerDownLeft = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* CornerDownRight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MainHeight = 64.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MainWidth = 64.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpEdgeWidth = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DownEdgeWidth = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeftEdgeWidth = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightEdgeWidth = 4.f;

};

struct FNaFramedImageSubwidgets {
	
	TSharedPtr<SGridPanel> GridPanel;

	TSharedPtr<SImage> ImageMain;

	TSharedPtr<SImage> ImageEdgeUp;

	TSharedPtr<SImage> ImageEdgeDown;

	TSharedPtr<SImage> ImageEdgeLeft;

	TSharedPtr<SImage> ImageEdgeRight;

	TSharedPtr<SImage> ImageCornerUpLeft;

	TSharedPtr<SImage> ImageCornerUpRight;

	TSharedPtr<SImage> ImageCornerDownLeft;

	TSharedPtr<SImage> ImageCornerDownRight;

};


/**
 * NaFramedImage is an image class which allows the width of border keep unchanged when resizing. 
 */
class NAWIDGETS_API SNaFramedImage : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaFramedImage)
	{}

	/* Image objects */

	SLATE_ATTRIBUTE(UObject*, MainImage)
	SLATE_ATTRIBUTE(UObject*, EdgeUp)
	SLATE_ATTRIBUTE(UObject*, EdgeDown)
	SLATE_ATTRIBUTE(UObject*, EdgeLeft)
	SLATE_ATTRIBUTE(UObject*, EdgeRight)
	SLATE_ATTRIBUTE(UObject*, CornerUpLeft)
	SLATE_ATTRIBUTE(UObject*, CornerUpRight)
	SLATE_ATTRIBUTE(UObject*, CornerDownLeft)
	SLATE_ATTRIBUTE(UObject*, CornerDownRight)

	/* Size parameters */

	SLATE_ATTRIBUTE(float, MainHeight)
	SLATE_ATTRIBUTE(float, MainWidth)
	SLATE_ATTRIBUTE(float, UpEdgeWidth)
	SLATE_ATTRIBUTE(float, DownEdgeWidth)
	SLATE_ATTRIBUTE(float, LeftEdgeWidth)
	SLATE_ATTRIBUTE(float, RightEdgeWidth)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FNaFramedImageParameters Params;

	FNaFramedImageSubwidgets Subwidgets;
};
