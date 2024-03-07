
#include <iostream>
#include <Windows.h>
#include "File.h"
#include <Psapi.h> 
#include <TlHelp32.h>
#include <filesystem>
#include <ShlObj.h>
#include <fstream>

namespace fs = std::filesystem;


#define VARIADIC(Param, ...) Param(__VA_ARGS__)
#define LOGI(...) std::printf(__VA_ARGS__)

char script[1000] = "powershell.exe New-Item -Path \"quido.txt\" -Value \"hello\"";

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
		std::printf("Injection successful!\n");	
	}
	else
	{
		std::printf("Injection unsuccessful!\n");
	}
}

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr)
{
	// Determine the length of the converted string 
	int strLength = WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1,
			nullptr, 0, nullptr, nullptr);

	// Create a std::string with the determined length 
	std::string str(strLength, 0);

	// Perform the conversion from LPCWSTR to std::string 
	WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],
		strLength, nullptr, nullptr);

	// Return the converted std::string 
	return str;
}

void TestCreateProcess() {
	STARTUPINFO             startupInfo;
	PROCESS_INFORMATION     processInformation;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(STARTUPINFO);
	if (CreateProcess(
		"C:\\Windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe",
		script,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		NULL,
		&startupInfo,
		&processInformation))
	{
		// InjectDLL(processInformation.hProcess);
		printf("Process created in a suspended state. Press any key to resume...\n");
		// getchar();
		ResumeThread(processInformation.hThread);
	}
}

void TestOpenProcess() {
	int id;	std::cin >> id;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);

	InjectDLL(hProcess);

}

int main()
{
	TestOpenProcess();
	// TestCreateProcess();
	// TrueCreateFileA("quido.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	/*fs::path pathToShow = "\\??\\C:\\Users\\LAP12512-local";
	
	std::cout << "exists() = " << fs::exists(pathToShow) << "\n"
		<< "root_name() = " << pathToShow.root_name().string() << "\n"
		<< "root_path() = " << pathToShow.root_path().string() << "\n"
		<< "root_directory() = " << pathToShow.root_directory().string() << "\n"
		<< "relative_path() = " << pathToShow.relative_path().string() << "\n"
		<< "parent_path() = " << pathToShow.parent_path().string() << "\n"
		<< "filename() = " << pathToShow.filename() << "\n"
		<< "stem() = " << pathToShow.stem() << "\n"
		<< "extension() = " << pathToShow.extension() << "\n";

	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
		std::cout << path << "\n";
		std::string relative_path = pathToShow.relative_path().string();
		if (relative_path.find(path, 0) != std::string::npos)
			std::cout << "User's folder: " << path << std::endl;
	}
	else {
		std::cerr << "Failed to get user's folder." << std::endl;
	}*/
	/*for (auto i = pathToShow.begin(); i != pathToShow.end(); i++) {
		std::cout << i->generic_string() << "\n";
	}*/

	/*PWSTR programDataPath;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &programDataPath);
	std :: cout << ConvertLPCWSTRToString(programDataPath).c_str() << "\n";
	std::cout << fs::temp_directory_path().string() << "\n";

	if (!strcmp(ConvertLPCWSTRToString(programDataPath).c_str(), pathToShow.relative_path().string().c_str())) {
		std::cout << "OK" << "\n";
	}
	SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &programDataPath);
	std::cout << ConvertLPCWSTRToString(programDataPath).c_str() << "\n";
	is_system(pathToShow);*/
	return 0;
}
