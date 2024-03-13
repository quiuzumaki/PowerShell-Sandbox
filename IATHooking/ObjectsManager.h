#pragma once

#include <Windows.h>
#include <map>
#include <typeinfo>

#include "Utils.h"

#ifndef OBJECTSMANAGER_H
#define OBJECTSMANAGER_H

#define MAX_SIZE 1000

class Object {
public:
	virtual LPCSTR getInfo() = 0;
	virtual void accept(class Visitor& v) = 0;
};

class ObjectFile : public Object {
private:
	LPCWSTR filename;
	PVOID lpBuffer;
	ULONG length;
public:
	ObjectFile(LPCWSTR filename, PVOID pBuffer = NULL, ULONG length = MAX_SIZE) {
		this->filename = filename;
		this->length = length;
		this->lpBuffer = (PBYTE)malloc(length);

		if (pBuffer != NULL) {
			memcpy(this->lpBuffer, pBuffer, length);
		}
	}
	void accept(Visitor& v);

	LPCSTR getFileName();
	LPCSTR getInfo();
	VOID setBuffer(PVOID);
	PVOID getBuffer();
	VOID setLength(ULONG);
	ULONG getLength();
};

class ObjectRegistry : public Object {
private:
	LPCSTR subKeyName;
	LPCSTR keyName;
public:
	ObjectRegistry(LPCSTR, LPCSTR);
	ObjectRegistry();
	void accept(Visitor& v);
	LPCSTR getInfo();
};

class Visitor {
public:
	virtual void visit(ObjectFile* ob) = 0;
	virtual void visit(ObjectRegistry* ob) = 0;
};

class ObjectVisitor : public Visitor {
public:
	void visit(ObjectFile* ob);
	void visit(ObjectRegistry* ob);
};

typedef std::pair<HANDLE, Object*> HandleEntry;
typedef std::map<HANDLE, Object*> HandleTable;

class ObjectsManager {
private:
	HandleTable* mHandleTable;
public:
	ObjectsManager();
	~ObjectsManager();
	BOOL isExist(HANDLE);
	Object* getObject(HANDLE);
	LPCSTR getObjectType(HANDLE);
	HANDLE insertObject(Object*);
	BOOL insertEntry(HANDLE, Object*);
	BOOL initObjectFileBuffer(HANDLE, PVOID, ULONG);
	BOOL insertEntry(HANDLE);
	BOOL deleteObject(HANDLE);
	DWORD getSize();
	BOOL isEmpty();
};

extern ObjectsManager* mObjectsManager;

#endif // !OBJECTSMANAGER_H
