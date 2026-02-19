#pragma once

#include "CoreMinimal.h"

/* TStack is a special type of array that can only add(push) and take(get/pop) elements from the end (stack top).
* I.e. a container using data structure of stack.
* As operation of array at the end takes only constant time [O(1)] while at the beginning takes time proportion to the length, 
* stack can act as a faster container of order-nonsensitive objects than normal array.
*/
template<typename T>
class TStack : private TArray<T> {

public:
	using TArray<T>::Num;
	using TArray<T>::RemoveAt;
	using TArray<T>::operator[];
	using TArray<T>::Add;

	TStack() {};

	// Amount of elements
	int Size() { return Num(); };

	// Push an element into stack
	void Push(T Elem) { Emplace(Elem); };

	// Get the top value of stack. This operation doesn't pop the element out.
	// Assert if empty!
	T GetTop() { 
		check(Num());
		return (*this)[Num() - 1];
	};

	// Pop an element from stack and return
	// Assert if empty!
	T Pop() {
		check(Num());
		T Out = GetTop;
		RemoveAt(Num() - 1);
		return Out;
	};

	// Pop the top element and push into another stack
	T PopInto(TStack<T> & Other) {
		Other.Push(Pop());
	};

	// True if the stack is empty
	bool IsEmpty() { return Num() == 0; };

};