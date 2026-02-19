// Fill out your copyright notice in the Description page of Project Settings.

#include "NaMob.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimeControlComponent.h"
#include "Combat/NaMobSkill.h"
#include "Combat/NaMobWeapon.h"
#include "Components/NaMobPlayerComponent.h"
#include "Components/NaMobSkillManager.h"
#include "Components/NaMobWeaponManager.h"
#include "Components/NaMobStateManager.h"
#include "Components/NaMobDataManager.h"
#include "Components/NaMobEnemyComponent.h"
#include "Combat/NaMobSkillCollision.h"
#include "Actors/HitDetector.h"
#include "Animation/AnimInstance.h"



/** Constructor & Input */

// Sets default values
ANaMob::ANaMob()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultMat = LoadObject<UMaterial>(nullptr, TEXT("/Engine/Content/EngineMaterials/WorldGridMaterial.WorldGridMaterial"));

	TimeControl = CreateDefaultSubobject<UTimeControlComponent>(TEXT("TimeControl"));
	SkillManager = CreateDefaultSubobject<UNaMobSkillManager>(TEXT("SkillManager"));
	WeaponManager = CreateDefaultSubobject<UNaMobWeaponManager>(TEXT("WeaponManager"));
	BasicStateManager = CreateDefaultSubobject<UNaMobBasicStateManager>(TEXT("BasicStateManager"));
	BasicStateManager_0 = BasicStateManager;
	TimeControl_0 = TimeControl;
}

// Called to bind functionality to input
void ANaMob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup if there's player component
	TArray<UActorComponent*> PlayerComps;
	GetComponents(UNaMobPlayerComponent::StaticClass(), PlayerComps);
	if (PlayerComps.Num() > 0)
		dynamic_cast<UNaMobPlayerComponent*>(PlayerComps[0])->SetupMobPlayerInput(PlayerInputComponent);


}




/** Original Construction */

// Construnction script
void ANaMob::OnConstruction(const FTransform & trans) {
	Super::OnConstruction(trans);
	
	UpdateSkeletalMesh();

	TimeControl = TimeControl_0;
	BasicStateManager = BasicStateManager_0;
	InitCapsuleMeshSize();
	UpdateAnimClass();
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
	Tick_CloseAnimSwitch();
}


/* Type check & get */

bool ANaMob::CheckTypeConflict() {
	TArray<UNaMobPlayerComponent*> PlayerComps;
	TArray<UNaMobEnemyComponent*> EnemyComps;
	GetComponents<UNaMobPlayerComponent>(PlayerComps);
	GetComponents<UNaMobEnemyComponent>(EnemyComps);

	if (PlayerComps.Num() > 1) {
		LogError("Mob type conflict: containing multiple player components.");
		return false;
	}
	else if (EnemyComps.Num() > 1) {
		LogError("Mob type conflict: containing multiple enemy components.");
		return false;
	}
	else if (PlayerComps.Num() + EnemyComps.Num() > 1) {
		LogError("Mob type conflict: containing conflicting type components.");
		return false;
	}

	return true;
}

bool ANaMob::AssertNoTypeConflict() {
	bool val = CheckTypeConflict();
	checkf(val, TEXT("Mob type conflict detected. See log for detail."));
	return val;
}

void ANaMob::GetAllRelatives(TArray<AActor*>& Out) {
	Out.Empty();
	Out.Emplace(this);
	TArray<ANaMobSkill*> Skills = SkillManager->GetAllSkills();
	Out.Append(Skills);		// Skill Actors
	Out.Append(WeaponManager->GetAllWeapons());		// Weapons

	TArray<ANaMobSkillCollision*> Cols;
	for (int i = 0; i < Skills.Num(); ++i) { // For all skills
		if (!IsValid(Skills[i]))
			continue;
		Cols = Skills[i]->GetCollisionSet_Safe().Array();	// Add collisions
		Out.Append(Cols);
		for (int j = 0; j < Cols.Num(); ++j) {
			if (!IsValid(Cols[j]))
				continue;
			Out.Emplace(Cols[j]->GetDetector()); // Add collision hit detectors
		}
	}
}

bool ANaMob::IsPlayerMob() {

#if WITH_EDITOR
	CheckTypeConflict();
#endif

	TArray<UNaMobPlayerComponent*> CompArray;
	GetComponents<UNaMobPlayerComponent>(CompArray);
	return CompArray.Num() == 1;

}

UNaMobPlayerComponent* ANaMob::GetPlayerComponent() {

#if WITH_EDITOR
	CheckTypeConflict();
#endif

	TArray<UNaMobPlayerComponent*> CompArray;
	GetComponents<UNaMobPlayerComponent>(CompArray);
	return (CompArray.Num() == 1) ? CompArray[0] : nullptr;

}

bool ANaMob::IsEnemyMob() {

#if WITH_EDITOR
	CheckTypeConflict();
#endif

	TArray<UNaMobEnemyComponent*> CompArray;
	GetComponents<UNaMobEnemyComponent>(CompArray);
	return CompArray.Num() == 1;

}

UNaMobEnemyComponent* ANaMob::GetEnemyComponent() {

#if WITH_EDITOR
	CheckTypeConflict();
#endif

	TArray<UNaMobEnemyComponent*> CompArray;
	GetComponents<UNaMobEnemyComponent>(CompArray);
	return (CompArray.Num() == 1) ? CompArray[0] : nullptr;

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

/* Basic actions */

void ANaMob::MobDie() {
	
	BasicStateManager->CurrentHP = 0;
	BasicStateManager->SetMovementType(ENaMobMovementType::MMT_NoMove);
	BasicStateManager->JumpType = ENaMobJumpType::MJT_NoJump;
	BasicStateManager->bIsDead = true;
	OnMobDying();

}
void ANaMob::DefaultMobResume() {
	BasicStateManager->CurrentHP = BasicStateManager->MaxHP;
	BasicStateManager->CurrentMP = BasicStateManager->MaxMP;
	BasicStateManager->SetMovementType(ENaMobMovementType::MMT_Run);
	BasicStateManager->JumpType = ENaMobJumpType::MJT_Default;
	BasicStateManager->bIsDead = false;
	OnMobResuming();
}

void ANaMob::CustomMobResume(int64 NewHP) {
	BasicStateManager->CurrentHP = NewHP;
	BasicStateManager->SetMovementType(ENaMobMovementType::MMT_Run);
	BasicStateManager->JumpType = ENaMobJumpType::MJT_Default;
	BasicStateManager->bIsDead = false;
	OnMobResuming();
	OnCustomMobResuming(NewHP);
}

void ANaMob::MobTakeDamage(int64 Damage) {
	BasicStateManager->CurrentHP -= Damage;

	if (BasicStateManager->CurrentHP <= 0)
		MobDie();

	OnMobTakingDamage(Damage);
}

/* Animation */

void ANaMob::UpdateAnimClass() {
	for (auto SkComp : SkeletalMeshComponents) {
		SkComp->SetAnimInstanceClass(AnimClass.Get());
	}
}

void ANaMob::SetAnimClass(TSubclassOf<UAnimInstance> NewClass) {
	
	AnimClass = NewClass;
	UpdateAnimClass();

}

/* Animation state switch */

void ANaMob::Tick_CloseAnimSwitch() {
	double Time = TimeControl->GetTime();
	TSet<FName> ToCloseNames;
	for (auto & Elem : AnimSwitchCloseTime)
		if (Time >= Elem.Value)
			ToCloseNames.Emplace(Elem.Key);
	for (auto & Name : ToCloseNames)
		CloseAnimStateSwitch(Name);
}

bool ANaMob::GetAnimStateSwitch(FName Key) {
	
	if (!this)
		return false;	// This check is for animation BPs in which this function will be called with null object

	bool* resptr = AnimStateSwitch.Find(Key);
	if (resptr)
		return *resptr;
	else
		return false;
}

void ANaMob::OpenAnimStateSwitch(FName Key, float DeltaTime) {
	// Set true
	AnimStateSwitch.FindOrAdd(Key) = true;
	// Set time to delay close
	if (DeltaTime > 0) 
		AnimSwitchCloseTime.FindOrAdd(Key) = TimeControl->GetTime() + (double)DeltaTime;
}

void ANaMob::CloseAnimStateSwitch(FName Key) {
	AnimStateSwitch.FindOrAdd(Key) = false;
	AnimSwitchCloseTime.Remove(Key);
}


