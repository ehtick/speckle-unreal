// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MaterialConverter.generated.h"

class URenderMaterial;
/**
 * 
 */
UCLASS()
class SPECKLEUNREAL_API UMaterialConverter : public UObject
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable)
		static void AssignPropertiesFromSpeckle(UMaterialInstanceDynamic* Material, const URenderMaterial* SpeckleMaterial);
	

};
