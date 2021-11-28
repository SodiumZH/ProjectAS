#pragma once

#include "CoreMinimal.h"


template<typename T>
class TStack : private TArray<T> {

public:

	TStack() {};

	// Amount of elements
	int Size() { return Num(); };

	// Push an element into stack
	void Push(T Elem) { Emplace(Elem); };

	T GetTop() { return (*this)[Num() - 1]; };

	// Pop an element from stack and return
	T Pop() {
		T Out = GetTop;
		(*this).RemoveAt(Num() - 1);
		return Out;
	};

	// Pop the top element and push into another stack
	T PopInto(TStack<T> & Other) {
		Other.Push(Pop());
	};

	// True if the stack is empty
	bool IsEmpty() { return Num() == 0; };

};