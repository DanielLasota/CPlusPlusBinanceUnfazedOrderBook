#pragma once

#include <windows.h>
#include <string>
#include <stdexcept>

struct MMapData {
    char* data = nullptr;
    size_t size = 0;
    HANDLE hFile = nullptr;
    HANDLE hMap = nullptr;
};

inline MMapData mmap_file(const std::string& path) {
    MMapData out{};

    HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Cannot open file: " + path);

    LARGE_INTEGER filesize;
    if (!GetFileSizeEx(hFile, &filesize)) {
        CloseHandle(hFile);
        throw std::runtime_error("Cannot get file size: " + path);
    }
    if (filesize.QuadPart == 0) {
        CloseHandle(hFile);
        throw std::runtime_error("File is empty: " + path);
    }

    HANDLE hMap = CreateFileMappingA(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!hMap) {
        CloseHandle(hFile);
        throw std::runtime_error("CreateFileMapping failed: " + path);
    }

    void* data = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    if (!data) {
        CloseHandle(hMap);
        CloseHandle(hFile);
        throw std::runtime_error("MapViewOfFile failed: " + path);
    }

    out.data = reinterpret_cast<char*>(data);
    out.size = static_cast<size_t>(filesize.QuadPart);
    out.hFile = hFile;
    out.hMap = hMap;
    return out;
}

inline void munmap_file(MMapData& mmapdata) {
    if (mmapdata.data) UnmapViewOfFile(mmapdata.data);
    if (mmapdata.hMap) CloseHandle(mmapdata.hMap);
    if (mmapdata.hFile) CloseHandle(mmapdata.hFile);
    mmapdata = MMapData{}; // zero all
}
