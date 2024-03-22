// Copyright 2024 Kitchen Mishap. All rights reserved.

#include "SpiralBuilderLibrary.h"
#include "Math/TransformNonVectorized.h"
#include "MathUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;


TArray<FTransform> USpiralBuilderLibrary::BuildRenderedJson(
	FVector meshOffset, float meshDivider, FString JsonPath, TArray<FVector>& colours, TArray<float>& opacities, TArray<FTransform>& hism1Transforms)
{
	string stdJsonPath = string(TCHAR_TO_UTF8(*JsonPath));

	TArray<FTransform> results;
	hism1Transforms.Empty();

	ifstream f(stdJsonPath);

	json jsonFile;
	f >> jsonFile;

	for (int i = 0; i < jsonFile.size(); i++)
	{
		auto asset = jsonFile[i]["asset"];
		float red = asset["r"];
		float green = asset["g"];
		float blue = asset["b"];
		float opacity = asset["a"];
		int hism = asset["h"];

		// Start with asset at the origin
		FTransform transform = FTransform::Identity;

		// Grab the axes while we can
		FVector forward = transform.GetScaledAxis(EAxis::X);
		FVector right = transform.GetScaledAxis(EAxis::Y);
		FVector up = transform.GetScaledAxis(EAxis::Z);

		transform.SetTranslation(-meshOffset/meshDivider);

		float scale = 1.0 / meshDivider;
		transform.SetScale3D(FVector(scale, scale, scale));

		auto transforms = jsonFile[i]["transform"];
		for (int t = 0; t < transforms.size(); t++)
		{
			auto transformName = transforms[t]["name"];
			float transformAmount = transforms[t]["amount"];

			FTransform primitiveTransform = FTransform::Identity;
			if (transformName == "ScaleX")
				primitiveTransform.SetScale3D(FVector(transformAmount, 1.0, 1.0));
			else if (transformName == "ScaleY")
				primitiveTransform.SetScale3D(FVector(1.0, transformAmount, 1.0));
			else if (transformName == "ScaleZ")
				primitiveTransform.SetScale3D(FVector(1.0, 1.0, transformAmount));
			else if (transformName == "TranslateX")
				primitiveTransform.SetTranslation(FVector(transformAmount, 0.0, 0.0));
			else if (transformName == "TranslateY")
				primitiveTransform.SetTranslation(FVector(0.0, transformAmount, 0.0));
			else if (transformName == "TranslateZ")
				primitiveTransform.SetTranslation(FVector(0.0, 0.0, transformAmount));
			else if (transformName == "RotateX")
			{
				FQuat4d rotate(forward, transformAmount * double(M_PI) / 180.0);
				primitiveTransform = primitiveTransform * rotate;
			}
			else if (transformName == "RotateY")
			{
				FQuat4d rotate(right, transformAmount * double(M_PI) / 180.0);
				primitiveTransform = primitiveTransform * rotate;
			}
			else if (transformName == "RotateZ")
			{
				FQuat4d rotate(up, transformAmount * double(M_PI) / 180.0);
				primitiveTransform = primitiveTransform * rotate;
			}
			transform = transform * primitiveTransform;
		}
		if (hism == 0)
		{
			results.Push(transform);
			colours.Push(FVector(red, green, blue));
			opacities.Push(opacity);
		}
		else
		{
			hism1Transforms.Push(transform);
		}
	}

	return results;
}
