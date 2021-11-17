// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshConverter.h"
#include "Objects/Mesh.h"
#include "SpeckleUnrealMesh.h"

#include "StaticMeshConverter.generated.h"

/**
 * 
 */
UCLASS()
class SPECKLEUNREAL_API UStaticMeshConverter : public UMeshConverter
{
	GENERATED_BODY()

public:

	/** You may specify a custom class inheriting from AASpeckleUnrealMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASpeckleUnrealMesh> MeshActor {
		ASpeckleUnrealMesh::StaticClass()
	};
	
	
	UStaticMeshConverter()
		:UMeshConverter()
	{ }

	virtual TSubclassOf<UBase> GetConversionType_Implementation() override
	{
		return UMesh::StaticClass();
	}
	
	virtual ASpeckleUnrealActor* ToNative_Implementation(const UBase* Base) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ASpeckleUnrealMesh* MeshToNative(const UMesh* SMesh);
	
	virtual ASpeckleUnrealMesh* MeshToNative_Implementation(const UMesh* SMesh);
	
};
