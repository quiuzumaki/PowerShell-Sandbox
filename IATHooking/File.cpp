#pragma once

#include "File.h"

//inline BOOL is_system(fs::path path) {
//
//}
//inline BOOL is_program_file(fs::path path) {
//	char env_path[MAX_PATH];
//	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_SYSTEM, NULL, 0, env_path))) {
//		
//		strcmp((char*)path.root_path().c_str(), env_path);
//		return TRUE;
//	}
//}

inline BOOL is_user_name(fs::path path);
inline BOOL is_local(fs::path path);
inline BOOL is_windows(fs::path path);

HANDLE HookCreateFileA(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
) {
	return TrueCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE HookCreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
) {
	MessageBox(NULL, "CreateFile", "Install Hook", NULL);
	return NULL;
}

BOOL HookWriteFile(
	HANDLE       hFile,
	LPCVOID      lpBuffer,
	DWORD        nNumberOfBytesToWrite,
	LPDWORD      lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
) {
	return TrueWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

BOOL HookWriteFileEx(
	HANDLE                          hFile,
	LPCVOID                         lpBuffer,
	DWORD                           nNumberOfBytesToWrite,
	LPOVERLAPPED                    lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
) {
	return TRUE;
}

BOOL HookReadFile(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
) {
	return TRUE;
}

BOOL HookReadFileEx(
	HANDLE                          hFile,
	LPVOID                          lpBuffer,
	DWORD                           nNumberOfBytesToRead,
	LPOVERLAPPED                    lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
) {
	return TRUE;
}

BOOL HookDeleteFileA(
	LPCSTR lpFileName
) {
	return TRUE;
}

BOOL HookDeleteFileW(
	LPCWSTR lpFileName
) {
	return TRUE;
}
