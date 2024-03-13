#include "ObjectsManager.h"

LPCSTR ObjectFile::getInfo() {
	return "ObjectFile";
}

VOID ObjectFile::setBuffer(PVOID pBuffer) {
	this->lpBuffer = (PBYTE)malloc(this->length);
	memcpy(this->lpBuffer, pBuffer, this->length);
}

VOID ObjectFile::setLength(ULONG length) {
	this->length = length;
}

PVOID ObjectFile::getBuffer() {
	return this->lpBuffer;
}

ULONG ObjectFile::getLength() {
	return this->length;
}

void ObjectFile::accept(Visitor &v) {
	v.visit(this);
}

ObjectRegistry::ObjectRegistry(LPCSTR keyName, LPCSTR subKeyName) : keyName(keyName), subKeyName(subKeyName) {}

ObjectRegistry::ObjectRegistry() {
	this->keyName = NULL;
	this->subKeyName = NULL;
}

LPCSTR ObjectRegistry::getInfo() {
	return "ObjectRegistry";
}

void ObjectRegistry::accept(Visitor& v) {
	v.visit(this);
}

// define visit method of ObjectVisitor
void ObjectVisitor::visit(ObjectFile* ob) {
	std::cout << "ObjectFile\n";
}

void ObjectVisitor::visit(ObjectRegistry* ob) {
	std::cout << "ObjectRegistry\n";
}


// define method of ObjectsManager
ObjectsManager::ObjectsManager() {
	this->mHandleTable = new HandleTable();
}

ObjectsManager::~ObjectsManager() {
	delete this->mHandleTable;
}

BOOL ObjectsManager::isExist(HANDLE handle) {
	if (this->isEmpty()) {
		return FALSE;
	}
	if (this->mHandleTable->find(handle) == this->mHandleTable->end()) {
		return FALSE;
	}
	return TRUE;
}

Object* ObjectsManager::getObject(HANDLE handle) {
	if (this->isEmpty()) {
		return 0;
	}

	return (*this->mHandleTable)[handle];
}

LPCSTR ObjectsManager::getObjectType(HANDLE handle) {
	if (handle == NULL || !this->isExist(handle)) {
		return NULL;
	}

	return (*this->mHandleTable)[handle]->getInfo();
}

inline HANDLE ObjectsManager::insertObject(Object* object) {
	HANDLE handle = NULL;

	if (handle == 0)
		return NULL;

	this->mHandleTable->insert(HandleEntry(handle, object));
	return handle;
}

BOOL ObjectsManager::insertEntry(HANDLE handle, Object* object) {
	if (handle == NULL || object == NULL)
		return FALSE;

	if (!strcmp(object->getInfo(), "ObjectFile")) {
		(*this->mHandleTable)[handle] = object;
	}
	else {
		(*this->mHandleTable)[handle] = object;
	}

	return TRUE;
}

BOOL ObjectsManager::initObjectFileBuffer(HANDLE handle, PVOID buffer, ULONG length) {
	if (handle == NULL || !this->isExist(handle))
		return FALSE;

	// Object* ob = (*this->mHandleTable)[handle];

	if (!strcmp(this->getObjectType(handle), "ObjectFile")) {

		((ObjectFile*)(*this->mHandleTable)[handle])->setLength(length);
		((ObjectFile*)(*this->mHandleTable)[handle])->setBuffer(buffer);
		
	}

	return TRUE;
}


inline BOOL ObjectsManager::deleteObject(HANDLE handle) {
	if (this->isEmpty()) {
		return FALSE;
	}
	if (this->isExist(handle)) {
		this->mHandleTable->erase(handle);
		return TRUE;
	}
	return FALSE;
}

inline DWORD ObjectsManager::getSize() {
	return this->mHandleTable->size();
}

inline BOOL ObjectsManager::isEmpty() {
	return this->mHandleTable->empty();
}

ObjectsManager* mObjectsManager = new ObjectsManager();