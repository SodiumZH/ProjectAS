// By Sodium


#include "Actors/NaDroppedItem.h"

// Sets default values
ANaDroppedItem::ANaDroppedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ANaDroppedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*
void ANaDroppedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
