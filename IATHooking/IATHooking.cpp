
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <ShlObj.h>
#include <fstream>
#include <map>

#include "File.h"
#include "Utils.h"
#include "Script.h"
#include "ObjectsManager.h"

namespace fs = std::filesystem;

#define VARIADIC(Param, ...) Param(__VA_ARGS__)
#define LOGI(...) std::printf(__VA_ARGS__)

void TestCreateProcess() {
	STARTUPINFO             startupInfo;
	PROCESS_INFORMATION     processInformation;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(STARTUPINFO);
	if (CreateProcessA(
		"C:\\Windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe",
		create_new_and_content,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED,
		NULL,
		"C:\\Users\\LAP12512-local",
		&startupInfo,
		&processInformation))
	{
		printf("Process created in a suspended state. Press any key to resume...\n");
		InjectDLL(processInformation.hProcess);
		ResumeThread(processInformation.hThread);
	}
}

void TestOpenProcess() {

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetProcessID("powershell.exe"));

	InjectDLL(hProcess);
}

VOID Test() {
	HANDLE tmp = (HANDLE)10;
	std::string c = "qaaaa";
	PVOID buffer = malloc(1000);
	memcpy(buffer, c.c_str(), c.size());

	ObjectFile* ob = new ObjectFile(L"quido", buffer, c.size());
	mObjectsManager->insertEntry(tmp, ob);
	// ObjectFile* new_ob = (ObjectFile*)mObjectsManager->getObject((HANDLE)10);
	Object* new_ob = mObjectsManager->getObject((HANDLE)10);

	std::cout << new_ob->getInfo();
}

void Test_1() {
	HANDLE tmp = (HANDLE)10;
	std::string c = "qaaaa";
	PVOID buffer = malloc(1000);
	memcpy(buffer, c.c_str(), c.size());

	ObjectFile* ob = new ObjectFile(L"quido");

	mObjectsManager->insertEntry(tmp, ob);

	mObjectsManager->initObjectFileBuffer(tmp, buffer, c.size());

	ObjectFile* new_ob = (ObjectFile*)mObjectsManager->getObject(tmp);
	
}

int main()
{
	/*std::map<int, std::string>* table = new std::map<int, std::string>();
	table->insert(std::pair<int, std::string>(1, "one"));
	table->insert(std::pair<int, std::string>(2, "two"));
	(*table)[3] = "three";
	for (int i = 1; i < table->size(); i++) {
		std::cout << (*table)[i] << "\n";
	}*/
	TestOpenProcess();
	/*std::string c = "qaaaa";
	PVOID buffer = malloc(1000);
	memcpy(buffer, c.c_str(), c.size());

	ObjectFile* ob = new ObjectFile(L"quido", buffer, 5);

	ObjectFile* a = (ObjectFile*)malloc(sizeof(ObjectFile));

	memcpy(a, ob, sizeof(ObjectFile));*/
	
	return 0;
}
