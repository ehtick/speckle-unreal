#pragma once

// json manipulation
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

// web requests
#include "Converter/MeshConverter.h"
#include "Runtime/Online/HTTP/Public/Http.h"

#include "GameFramework/Actor.h"
#include "SpeckleUnrealManager.generated.h"

class USpeckleUnrealLayer;
class IConverter;
UCLASS(BlueprintType)
class SPECKLEUNREAL_API ASpeckleUnrealManager : public AActor
{
	GENERATED_BODY()

	
public:
	
	FHttpModule* Http;
	

	UFUNCTION(CallInEditor, Category = "Speckle")
		void ImportSpeckleObject();

	UFUNCTION(CallInEditor, Category = "Speckle")
		void DeleteObjects();

	UFUNCTION(BlueprintCallable, Category = "Speckle")
		void AddConverter(UMeshConverter* MeshConverter);

	UFUNCTION(BlueprintCallable, Category = "Speckle")
		int32 RemoveConverter(FString& SpeckleType);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
		FString AuthToken {
		""
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speckle")
	bool ImportAtRuntime;

	
	TArray<USpeckleUnrealLayer*> SpeckleUnrealLayers;

	void OnStreamTextResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	/** Attempts to find an object in ASpeckleUnrealManager::SpeckleObjects with the specified id.
	 *  @param ObjectSpeckleId the Speckle ID of the desired object
	 *  @param OutObject the found object
	 *  @returns false if no object with ObjectSpeckleId is found 
	 */
	UFUNCTION()
	bool TryGetExistingNative(const FString& ObjectSpeckleId, UObject*& OutObject);

	/** Will parse the specified string as length unit
	 * @returns unit scale factor
	*/
	UFUNCTION()
	static float ParseScaleFactor(const FString& Units);

	/**
	 * @param ArrayField An array of Chunk objects
	 * @returns a flat array of the chunk objects' data
	 */
	TArray<TSharedPtr<FJsonValue>> CombineChunks(const TArray<TSharedPtr<FJsonValue>> * const ArrayField) const;
	
	// Sets default values for this actor's properties
	ASpeckleUnrealManager();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const TMap<FString, TSharedPtr<FJsonObject>> * const GetSpeckleObjects() const {return &SpeckleObjects; }
protected:
		
	TMap<FString, TSharedPtr<FJsonObject>> SpeckleObjects;

	TMap<FString, UObject*> CreatedObjects;
	TMap<FString, UObject*> InProgressObjects;

	TMap<FString, IConverter*> Converters;

	void ImportObjectFromCache(const TSharedPtr<FJsonObject> SpeckleObject, const TSharedPtr<FJsonObject> ParentObject = nullptr);
	
};