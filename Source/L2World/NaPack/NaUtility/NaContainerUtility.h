#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../NaGlobalHeader.h"
#include "NaContainerUtility.generated.h"

UCLASS()
class NAPACK_API UNaContainerUtility : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:


	/* Find key from given value in a map.
	* It will return a pointer pointing to the key. 
	* If the value has multiple keys, only one will be returned. 
	* If the value doesn't exist, return null pointer.
	* Please note that this action is generally slower than searching value from key, since it needs iteration.
	*/
	template<typename K, typename V>
	static K* FindKeyFromValue(TMap<K, V> & InMap, V Value){
		for (auto & Elem : InMap) {
			if (Elem.Value == Value) {
				return &(Elem.Key);
			}
		}
		return (K*)nullptr;
	};

	/* Find key from given value in a map. Output set of keys. 
	* Please note that this action is generally slower than searching value from key, since it needs iteration.
	*/
	template<typename K, typename V>
	static void FindAllKeysFromValue(TSet<K>& OutSet, const TMap<K, V>& InMap, V Value) {
		OutSet.Empty();
		for (auto & Elem : InMap) {
			if (Elem.Value == Value) {
				OutSet.Emplace(Elem.Key);
			}
		}
	};

	/* Find key from given value in a map. Output array of keys (order is random).
	* Please note that this action is generally slower than searching value from key, since it needs iteration.
	*/
	template<typename K, typename V>
	static void FindAllKeysFromValue(TArray<K>& OutArray, const TMap<K, V>& InMap, V Value) {
		OutArray.Empty();
		for (auto & Elem : InMap) {
			if (Elem.Value == Value) {
				OutArray.Emplace(Elem.Key);
			}
		}
	};

	/* Remove all pairs from in a map. Return whether something is removed.
	* Please note that this action is generally slower than removing from key, since it needs iteration.
	*/
	template<typename K, typename V>
	static bool RemoveValue(TMap<K, V> & InMap, V ValueToRemove) {
		TSet<K> RemoveKeys;
		FindAllKeysFromValue(RemoveKeys, InMap, ValueToRemove);
		for (auto & Elem : RemoveKeys) {
			InMap.Remove(Elem);
		}
		return RemoveKeys.Num() > 0;
	};

};