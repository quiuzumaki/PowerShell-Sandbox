#include "Logs.h"

Logs::Logs() {}

Logs::~Logs() {
	stream.close();
}

void Logs::open() {
	stream.open(this->filename, std::ofstream::binary);
}

BOOL Logs::is_open() {
	return this->stream.is_open();
}

void Logs::write(std::string buffer) {
	// this->stream.write(buffer.c_str(), buffer.length());
	this->stream << buffer << "\n";
}

void Logs::write(std::wstring buffer) {
	this->stream << ConvertLPCWSTRToString(buffer.c_str()) << "\n";
}

void Logs::write(LPCSTR format, ...) {
	char log[1000];
	va_list args;
	va_start(args, format);
	sprintf_s(log, sizeof(log), format, args);
	this->stream << log << "\n";
	va_end(args);
}

void Logs::write(LPCWSTR format, ...) {
	wchar_t log[256];
	va_list args;
	va_start(args, format);
	vswprintf(log, sizeof(log), format, args);
	this->stream << ConvertLPCWSTRToString(std::wstring(log).c_str()) << "\n";
	va_end(args);
}

void Logs::write(PBYTE buffer, ULONG length) {
	int x = length / 16;
	int y = length % 16;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < 16; j++) {
			this->stream << std::setfill('0') << std::setw(2) << std::hex << (int)buffer[16 * i + j] << " ";
		}
		this->stream << "     ";
		for (int j = 0; j < 16; j++) {
			this->stream << (char)buffer[16 * i + j] << " ";
		}
		this->stream << "\n";
	}

	for (int j = 0; j < y; j++) {
		this->stream << std::setfill('0') << std::setw(2) << std::hex << (int)buffer[16 * x + j] << " ";
	}
	this->stream << "     ";
	for (int j = 0; j < y; j++) {
		this->stream << (char)buffer[16 * x + j] << " ";
	}
	this->stream << "\n";
}


Logs* mLogs = new Logs();