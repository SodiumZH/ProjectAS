#pragma once
#include "BPLibraries/NaItemStatics.h"
#include "NaItemContainer.h"
#include "NaPublicDependencies/NaPublicDependencies.h"
#include "Components/NaGameModeItemSystemComponent.h"

void UNaItemStatics::FindEntryFromIndex(const FNaItemContainer& Target, int Position, ENaItemContainerFindingResult& ReturnType, UNaItemStack*& Result) {
	FNaItemContainerFindingReturn Return = Target.Find(Position);
	ReturnType = Return.Result;
	Result = (ReturnType == ENaItemContainerFindingResult::ICFR_Filled) ? Return.Stack : nullptr;
}

bool UNaItemStatics::ResizeContainer(FNaItemContainer& Target, int NewSize, bool bForce) {
	return Target.Resize(NewSize, bForce);
}

bool UNaItemStatics::AddItemEntry(FNaItemContainer& Target, int Position, UNaItemStack* Stack, bool bForce) {
	return Target.AddEntry(Position, Stack, bForce);
}

int UNaItemStatics::AddOrStackItemEntry(FNaItemContainer& Target, int Position, UNaItemStack* Stack) {
	return Target.AddOrStack(Position, Stack);
}

void UNaItemStatics::RemoveItemEntry(FNaItemContainer& Target, int Position) {
	Target.RemoveEntry(Position);
}

bool UNaItemStatics::MoveItemEntry(FNaItemContainer& Target, int From, int To, bool bForce) {
	return Target.MoveEntry(From, To, bForce);
}

void UNaItemStatics::SwapItemEntry(FNaItemContainer& Target, int P1, int P2) {
	Target.SwapEntry(P1, P2);
}

UNaGameModeItemSystemComponent* UNaItemStatics::GetGameModeItemSystemComponent(UObject* WorldContext) {
	return Cast<UNaGameModeItemSystemComponent>(UNaPublicDependencyStatics::GetNaGameModeSubunit(WorldContext, UNaGameModeItemSystemComponent::StaticClass()));
}