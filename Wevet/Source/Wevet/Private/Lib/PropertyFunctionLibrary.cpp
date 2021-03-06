// Copyright 2018 wevet works All Rights Reserved.

#include "Lib/PropertyFunctionLibrary.h"


void UPropertyFunctionLibrary::CopyAllObjectProperty(UObject* SourceObject, UObject* DestObject)
{
	const UClass* SourceClass = SourceObject->GetClass();
	const UClass* DestClass = DestObject->GetClass();

	if (DestObject->IsA(SourceClass))
	{
		for (const FProperty* SourceProperty : TFieldRange<const FProperty>(SourceClass))
		{
			FProperty* DestProperty = FindFProperty<FProperty>(DestClass, *SourceProperty->GetNameCPP());
			if (!DestProperty)
			{
				continue;
			}

			const void* Source = SourceProperty->ContainerPtrToValuePtr<void>(SourceObject);
			void* Dest = DestProperty->ContainerPtrToValuePtr<void>(DestObject);
			SourceProperty->CopySingleValue(Dest, Source);
		}
	}
}

void UPropertyFunctionLibrary::CopyObjectProperties(UObject* SourceObject, UObject* DestObject, const TArray<FString>& PropertyNames)
{
	const UClass* SourceClass = SourceObject->GetClass();
	const UClass* DestClass = DestObject->GetClass();
	if (DestObject->IsA(SourceClass))
	{
		for (const FString& PropertyName : PropertyNames)
		{
			FProperty* SourceProperty = FindFProperty<FProperty>(SourceClass, *PropertyName);
			if (!SourceProperty)
			{
				continue;
			}
			FProperty* DestProperty = FindFProperty<FProperty>(DestClass, *PropertyName);
			if (!DestProperty)
			{
				continue;
			}
			const void* Source = SourceProperty->ContainerPtrToValuePtr<void>(SourceObject);
			void* Dest = DestProperty->ContainerPtrToValuePtr<void>(DestObject);
			SourceProperty->CopySingleValue(Dest, Source);
		}
	}
}