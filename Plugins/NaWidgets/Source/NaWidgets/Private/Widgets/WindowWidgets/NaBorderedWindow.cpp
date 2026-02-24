// By Sodium

#include "Widgets/WindowWidgets/NaBorderedWindow.h"

TSharedRef<SWidget> UNaBorderedWindow::RebuildWidget()
{
	TSharedPtr<SWidget> ContentSlate;
	if (Content)
		ContentSlate = Content->TakeWidget();

	SAssignNew(BorderedWindow, SNaBorderedWindow)
		.Params(&Params)
		.MinBodySize(MinBodySize)
		.MaxBodySize(MaxBodySize)
		.Content(ContentSlate);

	return BorderedWindow.ToSharedRef();
}

void UNaBorderedWindow::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	BorderedWindow.Reset();
}

void UNaBorderedWindow::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (BorderedWindow.IsValid())
	{
		BorderedWindow->UpdateImages(Params);
		BorderedWindow->SetBorderWidths(Params.BorderTop, Params.BorderBottom, Params.BorderLeft, Params.BorderRight);
		BorderedWindow->SetBodySize(Params.BodySize);
	}
}

const FText UNaBorderedWindow::GetPaletteCategory()
{
	return FText::FromString(TEXT("NaWidgets"));
}

void UNaBorderedWindow::SetBodySize(FVector2D NewSize)
{
	Params.BodySize = NewSize;
	if (BorderedWindow.IsValid())
		BorderedWindow->SetBodySize(NewSize);
}

void UNaBorderedWindow::SetBorderWidths(float Top, float Bottom, float Left, float Right)
{
	Params.BorderTop    = Top;
	Params.BorderBottom = Bottom;
	Params.BorderLeft   = Left;
	Params.BorderRight  = Right;
	if (BorderedWindow.IsValid())
		BorderedWindow->SetBorderWidths(Top, Bottom, Left, Right);
}

FVector2D UNaBorderedWindow::GetBodySize() const
{
	return Params.BodySize;
}
