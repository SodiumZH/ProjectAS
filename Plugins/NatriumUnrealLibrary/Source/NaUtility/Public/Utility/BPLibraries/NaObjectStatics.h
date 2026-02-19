#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "NaObjectStatics.generated.h"


UCLASS()
class NAUTILITY_API UNaObjectStatics : public UBlueprintFunctionLibrary {
	
	GENERATED_BODY()

public:

	/* Pure C++ funcs */

	/* Cast an array to other type. Only for C++. If dynamic cast fails, it will return empty array. */
	template<typename To, typename From>
	static void DynamicCastArray(const TArray<From> & InArray, TArray<To> & OutArray) {
		OutArray.Empty();
		if (InArray.Num() == 0)
			return;
		for (int i = 0; i < InArray.Num(); ++i) {
			OutArray.Emplace(dynamic_cast<To>(InArray[i]));
			if (!OutArray[i] && InArray[i]) {
				OutArray.Empty();
				return;
			}
		}
	};

	/* Cast an array to other type. Only for C++. If dynamic cast fails, it will return empty array.
	This implementation is slower than the OutArray reference version.
	*/
	template<typename To, typename From>
	static TArray<To> DynamicCastArray(const TArray<From> & InArray) {
		TArray<To> OutArray;
		DynamicCastArray(InArray, OutArray);
		return OutArray;
	};

	/* Static cast an array to other type. Only for C++. It cost less than DynamicCastArray.
	Warning: Only used when you are sure casting will succeed! If casting is invalid, it will cause undefined behaviors!
	*/
	template<typename To, typename From>
	static void StaticCastArray(const TArray<From> & InArray, TArray<To> & OutArray) {
		OutArray.Empty();
		if (InArray.Num() == 0)
			return;
		for (int i = 0; i < InArray.Num(); ++i) {
			OutArray.Emplace(static_cast<To>(InArray[i]));
		}
	};

	/* Static cast an array to other type. Only for C++. It cost less than DynamicCastArray.
	Warning: Only used when you are sure casting will succeed! If casting is invalid, it will cause undefined behaviors!
	This implementation is slower than the OutArray reference version.
	*/
	template<typename To, typename From>
	static TArray<To> StaticCastArray(const TArray<From> & InArray) {
		TArray<To> OutArray;
		StaticCastArray(InArray, OutArray);
		return OutArray;
	};

	template<typename T>
	static bool ContainsSubclassObject_Templated(const TArray<T*> TestObjectArray, TSubclassOf<UObject> InClass) {
		if (TestObjectArray.Num() == 0)
			return false;
		TArray<UObject*> ArrayInObj;
		DynamicCastArray(TestObjectArray, ArrayInObj);
		return ContainsSubclassObject(ArrayInObj, InClass);
	};

	template<typename T>
	static bool ContainsSpecifiedClassObject_Templated(const TArray<T*> TestObjectArray, TSubclassOf<UObject> InClass) {
		if (TestObjectArray.Num() == 0)
			return false;
		TArray<UObject*> ArrayInObj;
		DynamicCastArray(TestObjectArray, ArrayInObj);
		return ContainsSpecifiedClassObject(ArrayInObj, InClass);
	};

public:


	/** Class related **/

	/* Test if the input object is subject of one of the classes in array. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Subclass Of (Class Array)", Keywords = "child inherit parent"), Category = "NaUtility|Objects")
	static bool IsSubclassOf_InArray(UObject* TestObject, const TArray<TSubclassOf<UObject>>& ClassArray);
	
	/* Test if the input object array contains at least one element which is subclass of specified class. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Contains Class", Keywords = "have subclass class object"), Category = "NaUtility|Objects")
	static bool ContainsSubclassObject(const TArray<UObject*> TestObjectArray, TSubclassOf<UObject> InClass);

	/* Test if the input object array contains at least one element which is right the specified class (excluding subclasses). */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Contains Specified Class", Keywords = "have class object"), Category = "NaUtility|Objects")
	static bool ContainsSpecifiedClassObject(const TArray<UObject*> TestObjectArray, TSubclassOf<UObject> InClass);


	/** Object Search **/

	/* Get a copy of attached components of a component. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get All Attached Components", Keywords = "subobject children attached component"), Category = "NaUtility|Objects")
	static void GetAllAttachedComponents(TArray<USceneComponent*>& OutComponents, USceneComponent* Target, TSubclassOf<USceneComponent> FilterClass, bool bIncludeGrandchildren = false);



};