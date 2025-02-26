#include "WinAPI.h"

FString UWinAPI::GetMachineGuid()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Cryptography"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        TCHAR buffer[256];
        DWORD size = sizeof(buffer);
        if (RegQueryValueEx(hKey, TEXT("MachineGuid"), nullptr, nullptr, (LPBYTE)buffer, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return FString(buffer);
        }
        RegCloseKey(hKey);
    }
    return TEXT("Unknown");
}
