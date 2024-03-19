
#include <iostream>
#include <Windows.h>
#include <Psapi.h> 
#include <TlHelp32.h>
#include <filesystem>
#include <yara.h>
#include <algorithm>

#include "Script.h"
#include "Util.h"
#include "HookFunctions.h"
#include "ObjectsManager.h"
#include "Logs.h"
#include "Blob.h"
#include "Rules.h"

using namespace std;

namespace fs = std::filesystem;

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

void InjectDLL(HANDLE hProcess) {
	LPVOID lpBaseAddress;
	const char* dllName = "C:\\Users\\ADMIN\\workspace\\Thesis\\MySandbox\\x64\\Release\\MySandbox.dll";
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

void TestCreateProcess() {
	STARTUPINFO             startupInfo;
	PROCESS_INFORMATION     processInformation;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(STARTUPINFO);

	if (CreateProcess(
		"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe",
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		"C:\\Users\\ADMIN",
		&startupInfo,
		&processInformation))
	{
		ResumeThread(processInformation.hThread);
		InjectDLL(processInformation.hProcess);
		// Sleep(2000);

		WaitForSingleObject(processInformation.hProcess, INFINITE);


		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);
	}
}

void test_match_blob(char* rule, uint8_t* buf, int sz) {
	yr_initialize();
	if (!matches_blob(rule, buf, sz, NULL, 0)) {
		std::cout << "error";
	}
	else {
		std::cout << "ok";
	}
	yr_finalize();
}

struct USER_DATA_CTX {
	bool match = 0;
	YR_META *meta;
};

int my_callback(
	YR_SCAN_CONTEXT* context,
	int message,
	void* message_data,
	void* user_data
) {
	
	switch (message)
	{
	case CALLBACK_MSG_RULE_MATCHING:
		((USER_DATA_CTX*)user_data)->match = TRUE;
		((USER_DATA_CTX*)user_data)->meta = ((YR_RULE*)message_data)->metas;
		break;
	}
	return CALLBACK_CONTINUE;
}

bool scan_memory_ex(char *my_rule, uint8_t *buffer, int size) {
	YR_RULES* rules;
	YR_RULE* rule;
	
	yr_initialize();

	if (compile_rule(my_rule, &rules) != ERROR_SUCCESS)
	{
		std::cout << "error";
		exit(EXIT_FAILURE);
	}

	USER_DATA_CTX ctx;
	void* user_data = &ctx;

	int scan_result = yr_rules_scan_mem(rules, buffer, size, SCAN_FLAGS_NO_TRYCATCH, my_callback, user_data, 0);

	if (scan_result != ERROR_SUCCESS)
	{
		std::cout << "error";
		exit(EXIT_FAILURE);
	}

	if (ctx.match) {
		std::cout << ctx.meta->string; 
	}

	yr_rules_destroy(rules);
	yr_finalize();

	return ctx.match;
}

bool scan_memory(PBYTE buffer, int size) {
	return scan_memory_ex(my_rules, (uint8_t*) buffer, size);
}

int main()
{	
	char rule[] = "\
		import \"pe\" \
		rule test { \
			meta: \
				description = \"Overlay Check\" \
			condition: \
				uint16(0) == 0x5a4d  \
		}";
	// TestCreateProcess();
	// TestOpenProcess();
	// test_save_load_rules();
	scan_memory_ex(is_rar, PE32_FILE, sizeof(PE32_FILE));
	// test_match_blob(rule, PE32_FILE, sizeof(PE32_FILE));
	return 0;
}
