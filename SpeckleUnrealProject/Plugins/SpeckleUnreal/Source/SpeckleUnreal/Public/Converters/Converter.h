// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpeckleUnrealActor.h"
#include "Objects/Base.h"
#include "UObject/Interface.h"
#include "Converter.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UConverter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPECKLEUNREAL_API IConverter
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<UBase> GetConversionType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ASpeckleUnrealActor* ToNative(const UBase* Base);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Clear();
};
