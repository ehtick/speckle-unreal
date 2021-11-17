#pragma once

// json manipulation
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

// web requests
#include "Runtime/Online/HTTP/Public/Http.h"

#include "Converters/Converter.h"
#include "SpeckleUnrealLayer.h"
#include "GameFramework/Actor.h"
#include "Objects/Kit.h"
#include "SpeckleUnrealManager.generated.h"


UCLASS(BlueprintType)
class SPECKLEUNREAL_API ASpeckleUnrealManager : public AActor
{
	GENERATED_BODY()

public:
	FHttpModule* Http;

	/* The actual HTTP call */
	UFUNCTION(CallInEditor, Category = "Speckle")
		void ImportSpeckleObject();

	UFUNCTION(CallInEditor, Category = "Speckle")
		void DeleteObjects();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
		FString ServerUrl {
		"https://speckle.xyz"
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
		FString StreamID {
		""
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
		FString ObjectID {
		""
	};

	/** A Personal Access Token can be created from your Speckle Profile page (Treat tokens like passwords, do not share publicly) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
		FString AuthToken {
		""
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
	bool ImportAtRuntime;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Conversion")
	FSpeckleKit Kit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle|Conversion", meta = (MustImplement = "Converter"))
	TSubclassOf<UObject> ToAdd;

	UFUNCTION(CallInEditor, Category = "Speckle|Conversion")
	void Add();


	void OnStreamTextResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// Sets default values for this actor's properties
	ASpeckleUnrealManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	float WorldToCentimeters;
	
	UFUNCTION(BlueprintPure)
	float ParseScaleFactor(const FString& Units) const;

	TArray<TSharedPtr<FJsonValue>> CombineChunks(const TArray<TSharedPtr<FJsonValue>>& ArrayField) const;

	TMap<FString, TSharedPtr<FJsonObject>> SpeckleObjects;

	size_t AddInprogressObject(UObject* Object)
	{
		return InProgressObjectsCache.Add(Object);
	}
	
protected:

	UWorld* World;
	
	TArray<UObject*> CreatedObjectsCache;
	TArray<UObject*> InProgressObjectsCache;
	
	
};