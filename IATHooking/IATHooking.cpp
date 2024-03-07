
#include <iostream>
#include <Windows.h>
#include "Hook.h"
#include "HookFunctions.h"
#include "ObjectsManager.h"
#include "Logs.h"
#include <Psapi.h> 
#include <TlHelp32.h>
#include <filesystem>

using namespace std;

#define VARIADIC(Param, ...) Param(__VA_ARGS__)

ULONGLONG GetProcessID(const char * process_name)
{
	HANDLE snapshot_handle;
	if ((snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
	{
		throw runtime_error("CreateToolhelp32Snapshot failed: " + to_string(GetLastError()));
	}

	ULONGLONG pid = -1;
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot_handle, &pe))
	{
		while (Process32Next(snapshot_handle, &pe))
		{
			if (!strcmp(pe.szExeFile, process_name))
			{
				pid = pe.th32ProcessID;
				break;
			}
		}
	}
	else
	{
		CloseHandle(snapshot_handle);
		throw runtime_error("Process32First failed: " + to_string(GetLastError()));
	}

	if (pid == -1)
	{
		CloseHandle(snapshot_handle);
		throw runtime_error("process not found");
	}

	CloseHandle(snapshot_handle);
	return pid;
}

MODULEINFO GetModuleInfo(HANDLE hProcess)
{
	MODULEINFO info = {};
	if (hProcess)
	{	
		HMODULE* modules = nullptr;
		unsigned long moduleCount = 0;
		EnumProcessModules(hProcess, modules, 0, &moduleCount);
		moduleCount = moduleCount / sizeof(HMODULE);

		modules = new HMODULE[moduleCount / sizeof(HMODULE)];
		char moduleName[64];
		EnumProcessModules(hProcess, modules, moduleCount * sizeof(HMODULE), &moduleCount);
		try {
			for (unsigned long i = 0; i < moduleCount; i++)
			{
				GetModuleBaseNameA(hProcess, modules[i], moduleName, sizeof(moduleName));
				if (_strcmpi(moduleName, "powershell.exe") == 0)
				{
					if (GetModuleInformation(hProcess, modules[i], &info, sizeof(info)))
						break;
					throw std::runtime_error("GetModuleInformation is error" + std::to_string(GetLastError()));
				}
				if (i == 0) // catches first module in case powershell.exe does not exist
				{
					GetModuleInformation(hProcess, modules[i], &info, sizeof(info));
				}
			}
		} catch (const std::exception& e) {
			std::cerr << e.what();
		}
	}
	return info;
}

/// <summary> 
/// Step 1: find PID of process 
/// Step 2: find Handle of process 
/// Step 3: get base address of module in process 
/// Step 4: read all bytes of module and save to a variable 
/// Step 5: parser and find IAT 
/// Step 6: write code to memory of process  
/// </summary> 

void InjectDLL(HANDLE hProcess) {
	LPVOID lpBaseAddress;
	const char* dllName = "D:\\WorkSpace\\Theis\\MySandbox\\x64\\Release\\MySandbox.dll";
	size_t sz = strlen(dllName);

	lpBaseAddress = VirtualAllocEx(hProcess, NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProcess, lpBaseAddress, dllName, sz, NULL);
	HMODULE hModule = GetModuleHandle("kernel32.dll");
	LPVOID lpStartAddress = GetProcAddress(hModule, "LoadLibraryA");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lpBaseAddress, 0, NULL);
	if (hThread)
	{
		printf("Injection successful!\n");
	}
	else
	{
		printf("Injection unsuccessful!\n");
	}
}

void TestOpenProcess() {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetProcessID("powershell.exe"));
	InjectDLL(hProcess);
}

void GetFilesInDirectory(std::vector<string>& out, const string& directory, bool getFolderNames)
{
	HANDLE dir;
	WIN32_FIND_DATAA file_data;

	if ((dir = FindFirstFileA((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

		if (file_name[0] == '.')
			continue;

		if (is_directory && !getFolderNames)
			continue;

		out.push_back(full_file_name);

	} while (FindNextFileA(dir, &file_data));

	FindClose(dir);
}

void TestLogs() {
	Logs mLogs;
	ObjectFile* mFile = new ObjectFile("qui.txt");

	// if (typeid(mFile) == typeid(otherFile)) 
	//	cout << typeid(ObjectFile*).name() << endl;
	mLogs.insertRecord("CreateFileA", mFile);
	mLogs.insertRecord("CreateFileA", new ObjectFile("tmp.txt"));
	mLogs.insertRecord("DeleteFileA", mFile);

	
	mLogs.getLogs();
}

void TestCreateFile(const vector<string> files, DWORD mode) {
	for (int i = 0; i < files.size(); i++)
		HANDLE hFile = CreateFileA(files[i].c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, mode, FILE_ATTRIBUTE_NORMAL, NULL);
}

void TestDeleteFile(const vector<string> files) {
	for (int i = 0; i < files.size(); i++)
		DeleteFileA(files[i].c_str());
}



void TestCreateProcess() {
	STARTUPINFO             startupInfo;
	PROCESS_INFORMATION     processInformation;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(STARTUPINFO);
	char param[20] = " Write-Host \"Hello\"";
	if (CreateProcess(
		"C:\\Users\\ADMIN\\Desktop\\powershell.exe",	
		//"C:\\Windows\\System32\\notepad.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL,
		&startupInfo,
		&processInformation))
	{
		InjectDLL(processInformation.hProcess);
		printf("Process created in a suspended state. Press any key to resume...\n");
		getchar();
		ResumeThread(processInformation.hThread);
	}
}

int main()
{
	// TestCreateProcess();
	TestOpenProcess();
	// printf("\n----------- Getting Sandbox Logs -----------\n");
	// SandboxLogs->getLogs();
	
	return 0;
}
