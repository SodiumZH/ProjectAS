#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"

USTRUCT(BlueprintType)
struct FDouble {

	GENERATED_USTRUCT_BODY()

protected:

	double Value;

public:

	/* Constructors */
	FDouble() { Value = 0; };

	FDouble(double Val) { Value = Val; };

	FDouble(float Val) { Value = (double)Val; };

	FDouble(int Val) { Value = (double)Val; };

	double Value() { return Value; }

	int Int() { return (int)Value; };

	float Float() { return (float)Value; };

	FDouble operator=(double Val) { this->Value = Val; return *this; };
	FDouble operator=(float Val) { this->Value = (double)Val; return *this; };
	FDouble operator=(int Val) { this->Value = (double)Val; return this; };

	FDouble operator+(FDouble Next) { return FDouble(Value + Next.Value); };
	FDouble operator-(FDouble Next) { return FDouble(Value - Next.Value); };
	FDouble operator*(FDouble Next) { return FDouble(Value * Next.Value); };
	FDouble operator/(FDouble Next) { return FDouble(Value / Next.Value); };
	FDouble operator%(FDouble Next) { return FDouble(Value % Next.Value); };

	FDouble operator+=(FDouble Next) { Value += Next.Value; return *this; };
	FDouble operator-=(FDouble Next) { Value -= Next.Value; return *this; };
	FDouble operator*=(FDouble Next) { Value *= Next.Value; return *this; };
	FDouble operator/=(FDouble Next) { Value /= Next.Value; return *this; };
	FDouble operator++(FDouble Next) { Value += 1.0; return *this; };
	FDouble operator--(FDouble Next) { Value -= 1.0; return *this; };

	bool operator==(FDouble Other) { return Value == Other.Value; };
	bool operator>(FDouble Other) { return Value > Other.Value; };
	bool operator<(FDouble Other) { return Value < Other.Value; };
	bool operator>=(FDouble Other) { return Value >= Other.Value; };
	bool operator<=(FDouble Other) { return Value <= Other.Value; };
};