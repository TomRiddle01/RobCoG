// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "RPid3d.h"
#include "RGrasp.h"
#include "RUtils.h"
#include "MotionControllerComponent.h"
#include "RCharacter.generated.h"


UCLASS()
class ROBCOG_API ARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
		
	// Left skeletal mesh component
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	USkeletalMeshComponent* LeftHand;
	
	// Left skeletal mesh bone to apply forces on
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	FName LeftControlBoneName;
	
	// Left finger bone collision (sensor) name
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	TArray<FName> LeftCollisionBoneNames;
	
	// Right skeltal mesh component
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	USkeletalMeshComponent* RightHand;
	
	// Left skeletal mesh bone to apply forces on
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	FName RightControlBoneName;
	
	// Right finger bone collision (sensor) name
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	TArray<FName> RightCollisionBoneNames;
	
	// Visualise or not target arrows
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	bool bVisTargetArrows;
	
	// PID controller proportional argument
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	float PGain;
	
	// PID controller integral argument
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	float IGain;
	
	// PID controller derivative argument
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	float DGain;
	
	// PID controller maximum output
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	float PIDMaxOutput;
	
	// PID controller minimum output
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	float PIDMinOutput;
	
	// Hand rotation controller output strength (multiply output velocity)
	UPROPERTY(EditAnywhere, Category = "Motion Controller")
	float RotOutStrength;
	
	// Finger types of the hand (make sure the skeletal bone name is part of the finger name)
	UPROPERTY(EditAnywhere, Category = "Joint Controller")
	TArray<ERHandLimb> FingerTypes;
	
	// Spring value to apply to the angular drive (Position strength)
	UPROPERTY(EditAnywhere, Category = "Joint Controller")
	float Spring;
	
	// Damping value to apply to the angular drive (Velocity strength) 
	UPROPERTY(EditAnywhere, Category = "Joint Controller")
	float Damping;
	
	// Limit of the force that the angular drive can apply
	UPROPERTY(EditAnywhere, Category = "Joint Controller")
	float ForceLimit;
	
	// Initial velocity
	UPROPERTY(EditAnywhere, Category = "Joint Controller")
	float Velocity;

	// Fixating grasp constraint instance
	UPROPERTY(EditAnywhere, Category = "Joint Controller")
	FConstraintInstance FixatingGraspConstraintInstance;
	
	// Handles moving forward/backward
	UFUNCTION()
	void MoveForward(const float Val);
	
	// Handles strafing Left/Right
	UFUNCTION()
	void MoveRight(const float Val);
	
	// Handles closing the left hand
	UFUNCTION()
	void CloseHandLeft(const float Val);
	
	// Handles opening the left hand
	UFUNCTION()
	void OpenHandLeft(const float Val);
	
	// Handles closing the left hand
	UFUNCTION()
	void CloseHandRight(const float Val);

	// Handles opening the left hand
	UFUNCTION()
	void OpenHandRight(const float Val);

	// Attach grasped object to hand
	UFUNCTION()
	void AttachHandLeft();

	// Attach grasped object to hand
	UFUNCTION()
	void AttachHandRight();

	// Callback on collision
	UFUNCTION()
	void OnHitLeft(UPrimitiveComponent* SelfComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	// Callback on collision
	UFUNCTION()
	void OnHitRight(UPrimitiveComponent* SelfComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// Switch between grasp types
	UFUNCTION()
	void OnSwitchGrasp();

	// Create constraint TEST TODO remove
	UFUNCTION()
	void OnCreateConstraint();

private:
	// Character camera
	UCameraComponent* CharCamera;

	// Motion controller origin
	USceneComponent* MCOriginComponent;
	
	// Left hand motion controller
	UMotionControllerComponent* LeftMC;
	
	// Right hand motion controller
	UMotionControllerComponent* RightMC;
	
	// Left target arrow visual
	UArrowComponent* LeftTargetArrow;
	
	// Right target arrow visual
	UArrowComponent* RightTargetArrow;
	
	// Left 3D PID controller for all axis
	FRPid3d LeftPID3D;
	
	// Right 3D PID controller for X axis
	FRPid3d RightPID3D;
	
	// Current location of the left control body
	FVector LeftCurrLoc;
	
	// Current location of the left control body
	FVector RightCurrLoc;
	
	// Current rotation of the left control body
	FQuat LeftCurrQuat;
	
	// Current rotation of the left control body
	FQuat RightCurrQuat;
	
	// Left control body (physics representation of an object)
	FBodyInstance* LeftControlBody;
	
	// Right control body (physics representation of an object)
	FBodyInstance* RightControlBody;
	
	// Left fingers type to constraints Map
	TMultiMap<ERHandLimb, FConstraintInstance*> LFingerTypeToConstrs;
	
	// Right fingers type to constraints Map
	TMultiMap<ERHandLimb, FConstraintInstance*> RFingerTypeToConstrs;
	
	// Left fingers type to collision sensor bone
	TMap<FName, FBodyInstance*> LFingerBoneNameToBody;
	
	// Right fingers type to collision sensor bone
	TMap<FName, FBodyInstance*> RFingerBoneNameToBody;
	
	// Grasp base class
	FRGrasp* LeftGrasp;
	
	// Grasp base class
	FRGrasp* RightGrasp;
	
	// Bone name to hand limb Map
	TMap<FName, ERHandLimb> BoneNameToLimbMap;

	// Finger hit events enable flag
	bool bLeftFingerHitEvents;

	// Finger hit events enable flag
	bool bRightFingerHitEvents;

	// Store a map of components in contact with fingers,
	// used for reasoning on what objects to attach to the hand
	// (e.g. Cup : Index, Middle, Pinky, Thumb; Table : Palm; -> Attach Cup )
	TMultiMap<AActor*, ERHandLimb> LeftHitActorToFingerMMap;

	// Store a map of components in contact with fingers,
	// used for reasoning on what objects to attach to the hand
	// (e.g. Cup : Index, Middle, Pinky, Thumb; Table : Palm; -> Attach Cup )
	TMultiMap<AActor*, ERHandLimb> RightHitActorToFingerMMap;

	// Left grasp physics constraint component (fixating grasp case)
	UPhysicsConstraintComponent* LeftGraspFixatingConstraint;

	// Right grasp physics constraint component (fixating grasp case)
	UPhysicsConstraintComponent* RightGraspFixatingConstraint;

	// Left fixating grasp static mesh (currently directly attaching to the hand is problematic)
	UStaticMeshComponent* LeftFixatingGraspStaticMesh;

	// Right fixating grasp static mesh (currently directly attaching to the hand is problematic)
	UStaticMeshComponent* RightFixatingGraspStaticMesh;

	// .H
	// Constraint component
	UPhysicsConstraintComponent* MConstraintComp;
	// Hand skeleton as fixed actor
	ASkeletalMeshActor* HandSkelAct;
	// Brown cylinder as fixed static mesh actor
	AStaticMeshActor* StaticMeshAct1;
	// Gray cylinder falling down and attaching the constraint to
	AStaticMeshActor* StaticMeshAct2;
	

	AStaticMeshActor* AttachedConeAct;
	AStaticMeshActor* GraspHelperStaticMeshAct;
	UStaticMeshComponent* GraspHelperStaticMesh;
	bool bAttached;

};
