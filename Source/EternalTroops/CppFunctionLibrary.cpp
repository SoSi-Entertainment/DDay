#include "CppFunctionLibrary.h"

int32 UCppFunctionLibrary::HashFromString(const FString& Input)
{
	return GetTypeHash(Input);
}
