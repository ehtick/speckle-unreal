﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Conversion/SpeckleTypeConverter.h"

#include "ProceduralMeshConverter.generated.h"

class UProceduralMeshComponent;
class UMesh;
class URenderMaterial;

UCLASS()
class SPECKLEUNREAL_API UProceduralMeshConverter :  public UObject, public ISpeckleTypeConverter
{
	GENERATED_BODY()

	CONVERTS_SPECKLE_TYPES()
	
protected:
	
	virtual AActor* CreateActor(const ASpeckleUnrealManager* Manager, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters = FActorSpawnParameters());
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MeshActorType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCreateCollisions;
	
	// Sets default values for this actor's properties
	UProceduralMeshConverter();

	
	virtual AActor* ConvertToNative_Implementation(const UBase* SpeckleBase, ASpeckleUnrealManager* Manager) override;
	virtual UBase* ConvertToSpeckle_Implementation(const UObject* Object, ASpeckleUnrealManager* Manager) override;
	
	UFUNCTION(BlueprintCallable)
	virtual AActor* MeshToNative(const UMesh* SpeckleMesh, ASpeckleUnrealManager* Manager);

	UFUNCTION(BlueprintCallable)
	virtual UMaterialInterface* GetMaterial(const URenderMaterial* SpeckleMaterial, ASpeckleUnrealManager* Manager);
	
	UFUNCTION(BlueprintCallable)
	virtual UMesh* MeshToSpeckle(const UProceduralMeshComponent* Object, ASpeckleUnrealManager* Manager);
	
};
