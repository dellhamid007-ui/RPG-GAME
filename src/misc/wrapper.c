#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>   // SHGetFolderPath, CSIDL_*

#include "wrapper.h"

bool getDocumentsPath(char outPath[MAX_PATH_LEN])
{
    if (!outPath) return false;

    HRESULT hr = SHGetFolderPathA(
        NULL,
        CSIDL_MYDOCUMENTS,
        NULL,
        SHGFP_TYPE_CURRENT,
        outPath
    );

    return SUCCEEDED(hr);
}


bool createDirectory(const char *path)
{
    if (!path) return false;

    // Try to create the directory
    if (CreateDirectoryA(path, NULL)) {
        return true; // created successfully
    }

    // If the directory already exists, treat as success
    DWORD err = GetLastError();
    if (err == ERROR_ALREADY_EXISTS) {
        return true;
    }

    // Any other error
    return false;
}

