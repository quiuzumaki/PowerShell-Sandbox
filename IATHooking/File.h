#pragma once

#ifndef FILE_H
#define FILE_H

#include <Windows.h>
#include <iostream>
#include "Logs.h"
#include <filesystem>
#include <ShlObj.h>

namespace fs = std::filesystem;

static HANDLE(*TrueCreateFileA) (
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	) = CreateFileA;

static HANDLE(*TrueCreateFileW) (
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	) = CreateFileW;

static BOOL(*TrueReadFile) (
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
	) = ReadFile;

static BOOL(*TrueWriteFile) (
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
	) = WriteFile;

static BOOL(*TrueDeleteFileW) (
	LPCWSTR lpFileName
	) = DeleteFileW;

HANDLE HookCreateFileA(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
);

HANDLE HookCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
);

BOOL HookWriteFile(
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
);

BOOL HookWriteFileEx(
	HANDLE                          hFile,
	LPCVOID                         lpBuffer,
	DWORD                           nNumberOfBytesToWrite,
	LPOVERLAPPED                    lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

BOOL HookReadFile(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
);

BOOL HookReadFileEx(
	HANDLE                          hFile,
	LPVOID                          lpBuffer,
	DWORD                           nNumberOfBytesToRead,
	LPOVERLAPPED                    lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

BOOL HookDeleteFileA(
	LPCSTR lpFileName
);

BOOL HookDeleteFileW(
	LPCWSTR lpFileName
);

#endif // !FILE_H
