#include "MD5Calculator.h"
#include "Misc/SecureHash.h"

FString UMD5Calculator::CalculateMD5(const FString& Input)
{
	const TCHAR* charInput = *Input;
	auto result = FMD5::HashAnsiString(charInput);

	return result;
}
