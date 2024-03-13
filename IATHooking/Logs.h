#pragma once

#include <Windows.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>


#include "Utils.h"

#ifndef LOGS_H
#define LOGS_H

#define TYPE(id) typeid(id).name();

class Logs {
private:
	LPCSTR filename = "Logs.txt";
	std::ofstream stream;
public:
	Logs();
	~Logs();
	void open();
	BOOL is_open();
	void write(std::string buffer);
	void write(std::wstring buffer);
	void write(LPCSTR format, ...);
	void write(LPCWSTR format, ...);
	void write(PBYTE buffer, ULONG length);
};

extern Logs* mLogs;

#endif // !LOGS_H