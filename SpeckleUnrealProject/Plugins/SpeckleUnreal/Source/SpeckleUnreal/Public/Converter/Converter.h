// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Converter.generated.h"

// This class does not need to be modified.
UINTERFACE()
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


	virtual UObject* ConvertToNative(const TSharedPtr<FJsonObject> Object, const TSharedPtr<FJsonObject> ParentObj = nullptr)
	{
		return nullptr;
	}
	
	/** The SpeckleType that this UConverter converts */
	virtual FString GetSpeckleType() const
	{
		return "";
	}
	
	virtual void DeleteObjects()
	{ }
};
