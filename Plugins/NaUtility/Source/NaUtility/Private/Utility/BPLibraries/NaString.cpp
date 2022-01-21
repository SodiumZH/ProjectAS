#pragma once

#include "Utility/BPLibraries/NaString.h"
#include "Kismet/KismetStringLibrary.h"
#include "Utility/DebugUtil/NaDebugUtility.h"

FName UNaStringLibrary::MakeUniqueName(FName RawName, const TArray<FName>& ExistingList) {

	FName Out = RawName;
	FString RawString = RawName.ToString();

	for (int i = 0; ExistingList.Contains(Out); ++i) {
		Out = FName(RawString + FString(TEXT("_")) + UKismetStringLibrary::Conv_IntToString(i));
		if (i < 0) {
			LogErrorNoContext("Make unique name failed: Expand index overflow. Maybe too many (> 2 billions) unique names have derived from one raw name.");
			return RawName;
		}
	}
	return Out;

}

bool UNaStringLibrary::IsDerivedUniqueName(FName Test, FName RawName) {

	// Transform to C-style string to iterate on its chars
	TCHAR* CString = (TCHAR*)*(Test.ToString());
	
	int i = Test.GetStringLength() - 1;

	// Empty or single-char string
	if (i <= 1)
		return false;

	// Find the last non-number char
	while (CString[i] >= '0' && CString[i] <= '9') {
		i--;
		if (i == -1)
			break;
	}

	// When i goes -1, the string is a pure numerical string, so false 
	if (i < 0)
		return false;

	// Now i is the index of the last non-number char
	if (CString[i] == '_')
		return true;	// It's OK if i==0, indicating that the string is like "_1234567" which is derived from empty string

	else return false;

}

FName UNaStringLibrary::FindRawName(FName Test) {
	
	// Transform to C-style string to iterate on its chars
	FName TestCpy = Test;
	TCHAR* CString = (TCHAR*)*(TestCpy.ToString());
	int i = Test.GetStringLength() - 1;

	// Empty or single-char string
	if (i <= 1)
		return Test;

	// Find the last non-number char
	while (CString[i] >= '0' && CString[i] <= '9') {
		i--;
		if (i == -1)
			break;
	}

	// When i goes -1, the string is a pure numerical string, so non-derived
	if (i == -1)
		return Test;

	// Now i is the index of the last non-number char
	if (CString[i] == '_') {
		CString[i] = '\0';	// Cut the string
		return FName(CString);	// Remake the name
	}

	else return Test;

}

FString UNaStringLibrary::Int2StrFixedLen(int InInt, int Len) {
	if (Len <= 0)
		return FString();
	int IsNeg = 0;
	if (InInt < 0) {
		IsNeg = 1;
		InInt = -InInt;
		Len++;	// Add an extra position for '-'
	}

	TCHAR* Str = new TCHAR[Len + 1];
	Str[Len] = '\0';
	Len--; // Now Len is for position of the C-string
	
	while (Len >= IsNeg) {
		Str[Len] = InInt % 10 + '0';
		Len--;
		InInt /= 10;
	}
	if (IsNeg)
		Str[0] = '-';
	FString Out = FString(Str);
	delete Str;
	return Out;
}

