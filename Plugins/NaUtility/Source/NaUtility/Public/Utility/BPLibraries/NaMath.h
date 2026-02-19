// Namespace NaMath is a collection of math utilities as pure C++ functions. No blueprint interface since most are implemented in kismet libraries.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaMath.generated.h"

UCLASS()
class NAUTILITY_API UNaMath : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:
	template <typename T>
	static T Max(T a, T b) { return a > b ? a : b; };

	template <typename T>
	static T Min(T a, T b) { return a > b ? b : a; };

	static float Clamp(float value, float min = 0, float max = 1);

	// Clamp a yaw value between -180~180 w/o changing its true value.
	static float YawClamp(float Yaw);

	// Clamp an angle between 0~180 w/o changing its true value.
	static float AngleClamp(float Angle);

	static float MoveTo(float from, float to, float max_step_length);

	static float AngularMoveTo_Deg(float from, float to, float max_step_length);

	static bool NearlyEqual(float a, float b, float toleration = KINDA_SMALL_NUMBER);
public:

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Identity (Transform)", Keywords = "identity zero"), Category = "Natrium|Utility|Math")
	static void IdentityTransform(FTransform & Value) { Value = FTransform::Identity; };

};