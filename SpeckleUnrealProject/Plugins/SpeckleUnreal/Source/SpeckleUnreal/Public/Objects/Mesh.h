// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base.h"

#include "Mesh.generated.h"

/**
 * 
 */
UCLASS()
class SPECKLEUNREAL_API UMesh : public UBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> Vertices;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> Faces;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector2D> TextureCoordinates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FMatrix Transform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URenderMaterial* RenderMaterial;
	
	virtual void Deserialize(const TSharedPtr<FJsonObject> Obj, const ASpeckleUnrealManager* Manager) override
	{
		Super::Deserialize(Obj, Manager);

		const float ScaleFactor = Manager->ParseScaleFactor(Units);

		//Parse optional Transform
		{
			Transform = FMatrix::Identity;
	
			const TArray<TSharedPtr<FJsonValue>>* TransformData = nullptr;
			if(Obj->HasField("properties") && Obj->GetObjectField("properties")->TryGetArrayField("transform", TransformData))
			{
				for(int32 Row = 0; Row < 4; Row++)
					for(int32 Col = 0; Col < 4; Col++)
					{
						Transform.M[Row][Col] = TransformData->operator[](Row * 4 + Col)->AsNumber();
					}
				Transform = Transform.GetTransposed();
				Transform.ScaleTranslation(FVector(ScaleFactor));
			}
		}

		//Parse Vertices
		{
			TArray<TSharedPtr<FJsonValue>> ObjectVertices = Manager->CombineChunks(Obj->GetArrayField("vertices"));
			const int32 NumberOfVertices = ObjectVertices.Num() / 3;
	
			Vertices.Reserve(NumberOfVertices);

			for (size_t i = 0, j = 0; i < NumberOfVertices; i++, j += 3)
			{
				Vertices.Add(Transform.InverseTransformPosition(FVector
				(
					ObjectVertices[j].Get()->AsNumber(),
					ObjectVertices[j + 1].Get()->AsNumber(),
					ObjectVertices[j + 2].Get()->AsNumber()
				) * ScaleFactor ));
			}
		}

		//Parse Faces
		{
			const TArray<TSharedPtr<FJsonValue>> FaceVertices = Manager->CombineChunks(Obj->GetArrayField("faces"));
			Faces.Reserve(FaceVertices.Num());
			for(const auto VertIndex : FaceVertices)
			{
				Faces.Add(VertIndex->AsNumber());
			}
		}

		//Parse TextureCoords
		{
			const TArray<TSharedPtr<FJsonValue>>* TextCoordArray;
			if(Obj->TryGetArrayField("textureCoordinates", TextCoordArray))
			{
				TArray<TSharedPtr<FJsonValue>> TexCoords = Manager->CombineChunks(*TextCoordArray);
		
				TextureCoordinates.Reserve(TexCoords.Num() / 2);
		
				for (int32 i = 0; i + 1 < TexCoords.Num(); i += 2)
				{
					TextureCoordinates.Add(FVector2D
					(
						TexCoords[i].Get()->AsNumber(),
						TexCoords[i + 1].Get()->AsNumber()
					));
				}
			}
		}
		
	}
};
