// Copyright 2024 Kitchen Mishap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SpiralBuilderLibrary.generated.h"

/**
 * A library that builds spirals and parts of spirals
 */
UCLASS(BlueprintType)
class USpiralBuilderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Spiral)
	static TArray<FTransform> BuildRenderedJson(
		FVector meshOffset, float meshDivider, FString JsonPath, TArray<FVector>& colours, TArray<float>& opacities, TArray<FTransform>& hism2Transforms);
};
