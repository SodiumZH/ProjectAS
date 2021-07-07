// Fill out your copyright notice in the Description page of Project Settings.


#include "NaMob.h"
#include "../NaUtility/NaUtility.h"
#include "Components/CapsuleComponent.h"


/** Constructor & Input */

// Sets default values
ANaMob::ANaMob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called to bind functionality to input
void ANaMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




/** Original Construction */

// Construnction script
void ANaMob::OnConstruction(const FTransform & trans) {
	Super::OnConstruction(trans);
	
	UpdateSkeletalMesh();
	InitCapsuleMeshSize();
	1;
}

// Called when the game starts or when spawned
void ANaMob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANaMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




/////////////////////
/** Skeletal Mesh **/
/////////////////////


/* Mesh */

void ANaMob::ClearSkeletalMesh() {
	
	int MeshCount = SkeletalMeshComponents.Num();
	if (MeshCount == 0)
		return;

	for (int i = 0; i < SkeletalMeshComponents.Num(); ++i) {
		if (IsValid(SkeletalMeshComponents[i]))
			SkeletalMeshComponents[i]->DestroyComponent();
	}

	SkeletalMeshComponents.Empty();
	return;
}

void ANaMob::UpdateSkeletalMesh() {
	
	ClearSkeletalMesh();

	USkeletalMeshComponent* temp = nullptr;
	OriginalMats.Empty(); // Clear old original materials
	
	for (int i = 0; i < SkeletalMeshes.Num(); ++i) {
		FString CompName = TEXT("SkeletalMesh");
		CompName = CompName + FString::Printf(TEXT("%d"), i);
		temp = NewObject<USkeletalMeshComponent>(this, *CompName);
		temp->RegisterComponent();
		temp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		temp->SetSkeletalMesh(SkeletalMeshes[i]);
		SkeletalMeshComponents.Add(temp);
		OriginalMats.Append(temp->GetMaterials());// Record original materials
	}

	// Materials should be updated too
	UpdateMaterials();
}

void ANaMob::ResetSkMeshComponents(const TArray<USkeletalMesh*> & NewMeshes) {
	SkeletalMeshes = NewMeshes;
	UpdateSkeletalMesh();
}


/* Materials */

TArray<int> ANaMob::DecodeMatMapping(const TArray<int> & input) { 

	// Count total mat socket counts of all skeletal meshes
	int TotalCount = 0;
	for (int i = 0; i < SkeletalMeshComponents.Num(); ++i) 
		TotalCount += SkeletalMeshComponents[i]->GetMaterials().Num();

	// Decode
	TArray<int> Out = TArray<int>();
	TArray<int> temp = TArray<int>();
	int LastElem = 0;
	for (int i = 0; i < input.Num(); ++i) {
		
		// In case it's a number
		if (input[i] >= 0) {
			Out.Add(input[i]);
			LastElem = input[i];
		}
		// In case it's negative that represents repeat
		else {
			temp.Init(LastElem, -input[i]);
			Out.Append(temp);
		}
	}

	// Resize to mat count
	// Longer -> cut
	while (Out.Num() > TotalCount) {
		Out.RemoveAt(Out.Num() - 1); // This operation doesn't cause element movement
	}
	// Shorter -> repeat the last element
	while (Out.Num() < TotalCount) {
		Out.Add(LastElem);
	}

	return Out;
}

void ANaMob::ApplyMaterials_Unsafe(const TArray<UMaterialInterface*> & Mat, const TArray<int> & DecodedMap) {

	USkeletalMeshComponent* temp = nullptr;
	int MeshCount;
	int MapIndex = 0;

	for (int i = 0; i < SkeletalMeshComponents.Num(); ++i) { // Iterate on all sk-meshes
		temp = SkeletalMeshComponents[i];
		MeshCount = temp->GetMaterials().Num();
		for (int j = 0; j < MeshCount; ++j) { // Iterate on all mat sockets for a single sk-mesh
			temp->SetMaterial(j,
				(Mat.Num() <= DecodedMap[MapIndex]) ? DefaultMat : Mat[DecodedMap[MapIndex]]
				);
			MapIndex++;
		}
	}
}

void ANaMob::UpdateMaterials() {
	TArray<int> MapDecoded = DecodeMatMapping(MatMapping);
	ApplyMaterials_Unsafe(Materials, MapDecoded);
}

void ANaMob::InitializeMaterials() {
	int matindex = 0;
	for (int i = 0; i < SkeletalMeshComponents.Num(); ++i) {
		for (int j = 0; j < SkeletalMeshComponents[i]->GetMaterials().Num(); ++j) {
			SkeletalMeshComponents[i]->SetMaterial(j, OriginalMats[matindex]);
			++matindex;
		}
	}
	return;
}

void ANaMob::ResetEnableMatOverride(bool NewEnable) {
	if (NewEnable)
		UpdateMaterials();
	else InitializeMaterials();
	bEnableMaterialOverride = NewEnable;
}

void ANaMob::ResetMaterials(const TArray<UMaterialInterface*> & NewMats, const TArray<int> & NewMapping) {
	if (!bEnableMaterialOverride)
		return;
	Materials = NewMats;
	MatMapping = NewMapping;
	UpdateMaterials();
}


/* Transform */

// Capsule //

void ANaMob::UpdateCapsule() {
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleHalfHeight(MOB_DEFAULT_HEIGHT * CapsuleHeightScale / 2.f);
	Capsule->SetCapsuleRadius(MOB_DEFAULT_DIAMETER * CapsuleDiameterScale / 2.f);
	return;
}

void ANaMob::ResetCapsuleSize(float HeightScale, float DiameterScale) {
	CapsuleHeightScale = HeightScale;
	CapsuleDiameterScale = DiameterScale;
	UpdateCapsule();
}

void ANaMob::SetCapsuleScale(float HeightScale, float DiameterScale) {
	ResetCapsuleSize(HeightScale, DiameterScale);
}

// Mesh //

FTransform ANaMob::GetMeshTransApplied() {
	FTransform MeshTransApplied = MeshOffset;
	if (bLockMeshScale) {
		MeshTransApplied.SetScale3D(FVector(LockedScale3D, LockedScale3D, LockedScale3D));
	}
	else if (bLockMeshScaleXY) {
		float Z = MeshTransApplied.GetScale3D().Z;
		MeshTransApplied.SetScale3D(FVector(LockedScaleXY, LockedScaleXY, Z));
	}
	return MeshTransApplied;
}

void ANaMob::UpdateMeshTransform() {
	GetMesh()->SetRelativeTransform(GetMeshTransApplied());
}

void ANaMob::ResetMeshTransform(FTransform Trans) {
	MeshOffset = Trans;
	if (bLockMeshScale)
		LockedScale3D = Trans.GetScale3D().X;
	else if (bLockMeshScaleXY)
		LockedScaleXY = Trans.GetScale3D().X;
	UpdateMeshTransform();
}

void ANaMob::SetMeshOffset(FTransform InTrans) {

	ResetMeshTransform(InTrans);
}

void ANaMob::SetMeshOffset_LRS(FVector Location, FRotator Rotation, FVector Scale) {
	ResetMeshTransform(FTransform(Rotation, Location, Scale));
}

void ANaMob::SetMeshLocation(FVector InLoc) {
	MeshOffset.SetLocation(InLoc);
	UpdateMeshTransform();
}

void ANaMob::SetMeshRotation(FRotator InRot) {
	MeshOffset.SetRotation(InRot.Quaternion());
	UpdateMeshTransform();
}

void ANaMob::SetMeshScale(FVector InScale) {
	MeshOffset.SetScale3D(InScale);
	UpdateMeshTransform();
}

void ANaMob::InitCapsuleMeshSize() {
	UpdateCapsule();
	UpdateMeshTransform();
}