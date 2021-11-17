// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

#include "Kit.generated.h"

class IConverter;
class ASpeckleUnrealManager;


USTRUCT(BlueprintType)
struct FSpeckleKit
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UObject*> Converters;
	
};


/**
 * 
 */
UCLASS(BlueprintType)
class SPECKLEUNREAL_API UKit : public UObject
{
	GENERATED_BODY()

public:
	FSpeckleKit kit;
	
	UFUNCTION(BlueprintPure)
	bool CanConvert(const FString& SpeckleType) const
	{
		return false;
		return kit.Converters.Contains(SpeckleType);
	}
	
	void ToNative(AActor* AOwner, ASpeckleUnrealManager* Manager, const TSharedPtr<FJsonObject> SpeckleObject, const TSharedPtr<FJsonObject> ParentObject = nullptr);

	UFUNCTION(BlueprintCallable)
	void Clear();
};