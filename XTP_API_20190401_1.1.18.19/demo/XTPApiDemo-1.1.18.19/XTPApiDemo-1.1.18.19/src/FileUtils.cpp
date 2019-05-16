#include "FileUtils.h"
#include <stdarg.h>
#include <stdio.h>

#define CC_BREAK_IF(cond)           if(cond) break
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = NULL; } } while(0)

FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
}

bool FileUtils::init()
{
	return readJson("config.json", m_docData);
}

unsigned char* FileUtils::getFileData(const std::string& filename, const char* mode, size_t *size)
{
	unsigned char * buffer = NULL;
	*size = 0;
	do
	{
		// read the file from hardware
		const std::string fullPath = filename;//fullPathForFilename(filename);
		FILE *fp = fopen(fullPath.c_str(), mode);
		if (!fp)
		{
			return NULL;
		}

		fseek(fp, 0, SEEK_END);
		*size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buffer = (unsigned char*)malloc(*size);
		*size = fread(buffer, sizeof(unsigned char), *size, fp);
		fclose(fp);
	} while (0);

	return buffer;
}

bool FileUtils::readJson(const char *pszFileName, rapidjson::Document &doc)
{
	bool bRet = false;
	size_t size = 0;
	unsigned char *pBytes = NULL;
	do {
		CC_BREAK_IF(pszFileName == NULL);
		std::string jsonpath = pszFileName;//FileUtils::getInstance()->fullPathForFilename(pszFileName);
		pBytes = getFileData(jsonpath.c_str(), "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(doc.HasParseError());
		bRet = true;
	} while (0);

	return bRet;
}

std::string FileUtils::stdStringForKey(const char *keyPathFormat, ...)
{
	char keyPath[255];
	va_list args;
	va_start(args, keyPathFormat);
#ifdef _WIN32
	vsprintf_s(keyPath, keyPathFormat, args);
#else
	vsprintf(keyPath, keyPathFormat, args);
#endif // _WIN32
	va_end(args);

	std::string strKeyPath = keyPath;

	const rapidjson::Value& dict = objectForKey(m_docData, strKeyPath);

	if (dict.IsNull())
	{
		return "";
	}

	if (!dict.IsString()) {
		return "";// strKeyPath;
	}

	std::string ret = dict.GetString();

	return ret;
}

int FileUtils::intForKey(const char *keyPathFormat, ...)
{
	char keyPath[255];
	va_list args;
	va_start(args, keyPathFormat);
#ifdef _WIN32
	vsprintf_s(keyPath, keyPathFormat, args);
#else
	vsprintf(keyPath, keyPathFormat, args);
#endif // _WIN32
	va_end(args);

	std::string strKeyPath = keyPath;

	const rapidjson::Value& dict = objectForKey(m_docData, strKeyPath);

	if (dict.IsNull())
	{
		return 0;
	}

	if (!dict.IsInt()) {
		return 0;
	}

	int ret = dict.GetInt();

	return ret;
}

int FileUtils::countForKey(const char * keyPathFormat, ...)
{
	char keyPath[255];
	va_list args;
	va_start(args, keyPathFormat);
#ifdef _WIN32
	vsprintf_s(keyPath, keyPathFormat, args);
#else
	vsprintf(keyPath, keyPathFormat, args);
#endif // _WIN32
	va_end(args);

	std::string strKeyPath = keyPath;

	const rapidjson::Value& dict = objectForKey(m_docData, strKeyPath);

	if (dict.IsNull())
	{
		return 0;
	}

	if (!dict.IsArray())
	{
		return 0;
	}

	int nRet = (int)(dict.Size());

	return nRet;
}

double FileUtils::floatForKey(const char * keyPathFormat, ...)
{
	char keyPath[255];
	va_list args;
	va_start(args, keyPathFormat);
#ifdef _WIN32
	vsprintf_s(keyPath, keyPathFormat, args);
#else
	vsprintf(keyPath, keyPathFormat, args);
#endif // _WIN32
	va_end(args);

	std::string strKeyPath = keyPath;

	const rapidjson::Value& dict = objectForKey(m_docData, strKeyPath);

	if (dict.IsNull())
	{
		return 0;
	}

	if (!dict.IsDouble()) {
		return 0;
	}

	double ret = dict.GetDouble();

	return ret;
}

bool FileUtils::boolForKey(const char * keyPathFormat, ...)
{
	char keyPath[255];
	va_list args;
	va_start(args, keyPathFormat);
#ifdef _WIN32
	vsprintf_s(keyPath, keyPathFormat, args);
#else
	vsprintf(keyPath, keyPathFormat, args);
#endif // _WIN32
	va_end(args);

	std::string strKeyPath = keyPath;

	const rapidjson::Value& dict = objectForKey(m_docData, strKeyPath);

	if (dict.IsNull())
	{
		return false;
	}

	if (!dict.IsBool()) {
		return false;
	}

	bool ret = dict.GetBool();

	return ret;
}

const rapidjson::Value& FileUtils::objectForKey(const rapidjson::Value &root, const std::string &keyPath)
{

	if (root.IsNull()) {
		return root;
	}

	std::string strKeyPath = keyPath;

	if (strKeyPath.length() == 0)
	{
		return root;
	}

	size_t pos = keyPath.find_first_of(".");
	size_t pos2 = keyPath.find_first_of("[");

	bool findKey = pos != std::string::npos;
	bool findArray = pos2 != std::string::npos;

	if (findKey && findArray) {
		if (pos > pos2) {
			findKey = false;
		}
		else {
			findArray = false;
		}
	}

	if (findKey) {
		//find "."
		std::string key = keyPath.substr(0, pos);
		strKeyPath = strKeyPath.substr(pos + 1, keyPath.length() - 1);

		if (key.length() == 0) {
			return objectForKey(root, strKeyPath);
		}

		const rapidjson::Value &dict = getSubDictionary_json(root, key.c_str());
		return objectForKey(dict, strKeyPath);
	}

	if (findArray) {
		//find "[",should find "]"
		size_t pos3 = keyPath.find_first_of("]");
		if (pos3 == std::string::npos || pos2 >= pos3) {

			//CCLOG("error index in config key");

			return root;
		}

		std::string key = keyPath.substr(0, pos2);
		std::string strIndex = keyPath.substr(pos2 + 1, pos3 - pos2 - 1);

		int idx = atoi(strIndex.c_str());

		strKeyPath = strKeyPath.substr(pos3 + 1, keyPath.length() - 1);

		if (key.length() == 0) {
			const rapidjson::Value &dict = getSubDictionary_json(root, idx);

			return objectForKey(dict, strKeyPath);
		}
		else {
			const rapidjson::Value &dict = getSubDictionary_json(root, key.c_str(), idx);

			return objectForKey(dict, strKeyPath);
		}

	}

	if (!checkObjectExist_json(root, keyPath.c_str()))
	{
		return root;
	}

	return getSubDictionary_json(root, keyPath.c_str());

}


const rapidjson::Value& FileUtils::getSubDictionary_json(const rapidjson::Value &root, const char* key)
{
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		return root[key];
	} while (0);
	return root;
}

const rapidjson::Value& FileUtils::getSubDictionary_json(const rapidjson::Value &root, const char* key, int idx)
{
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		CC_BREAK_IF(root[key].IsNull());
		CC_BREAK_IF(!root[key].IsArray());
		CC_BREAK_IF(!checkObjectExist_json(root[key], idx));
		CC_BREAK_IF(root[key][idx].IsNull());
		return root[key][idx];
	} while (0);
	return root;
}

const rapidjson::Value& FileUtils::getSubDictionary_json(const rapidjson::Value &root, int idx)
{
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.IsArray());
		CC_BREAK_IF(!checkObjectExist_json(root, idx));
		CC_BREAK_IF(root[idx].IsNull());
		return root[idx];
	} while (0);
	return root;
}

int FileUtils::getIntValue_json(const rapidjson::Value& root, const char* key, int def)
{
	int nRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		CC_BREAK_IF(root[key].IsNull());
		nRet = root[key].GetInt();
	} while (0);

	return nRet;
}


float FileUtils::getFloatValue_json(const rapidjson::Value& root, const char* key, float def)
{
	float fRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		CC_BREAK_IF(root[key].IsNull());
		fRet = (float)root[key].GetDouble();
	} while (0);

	return fRet;
}

bool FileUtils::getBooleanValue_json(const rapidjson::Value& root, const char* key, bool def)
{
	bool bRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		CC_BREAK_IF(root[key].IsNull());
		bRet = root[key].GetBool();
	} while (0);

	return bRet;
}

const char* FileUtils::getStringValue_json(const rapidjson::Value& root, const char* key, const char *def)
{
	const char* sRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		CC_BREAK_IF(root[key].IsNull());
		sRet = root[key].GetString();
	} while (0);

	return sRet;
}



int FileUtils::getArrayCount_json(const rapidjson::Value& root, const char* key, int def)
{
	int nRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(key));
		CC_BREAK_IF(root[key].IsNull());
		nRet = (int)(root[key].Size());
	} while (0);

	return nRet;
}


int FileUtils::getIntValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, int def)
{
	int nRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(arrayKey));
		CC_BREAK_IF(root[arrayKey].IsNull());
		CC_BREAK_IF(!root[arrayKey].IsArray());
		CC_BREAK_IF(!checkObjectExist_json(root[arrayKey], idx));
		CC_BREAK_IF(root[arrayKey][idx].IsNull());
		nRet = root[arrayKey][idx].GetInt();
	} while (0);

	return nRet;
}


float FileUtils::getFloatValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, float def)
{
	float fRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(arrayKey));
		CC_BREAK_IF(root[arrayKey].IsNull());
		CC_BREAK_IF(!root[arrayKey].IsArray());
		CC_BREAK_IF(!checkObjectExist_json(root[arrayKey], idx));
		CC_BREAK_IF(root[arrayKey][idx].IsNull());
		fRet = (float)root[arrayKey][idx].GetDouble();
	} while (0);

	return fRet;
}

bool FileUtils::getBoolValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, bool def)
{
	bool bRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(arrayKey));
		CC_BREAK_IF(root[arrayKey].IsNull());
		CC_BREAK_IF(!root[arrayKey].IsArray());
		CC_BREAK_IF(!checkObjectExist_json(root[arrayKey], idx));
		CC_BREAK_IF(root[arrayKey][idx].IsNull());
		bRet = root[arrayKey][idx].GetBool();
	} while (0);

	return bRet;
}

const char* FileUtils::getStringValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, const char *def)
{
	const char *sRet = def;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.HasMember(arrayKey));
		CC_BREAK_IF(root[arrayKey].IsNull());
		CC_BREAK_IF(!root[arrayKey].IsArray());
		CC_BREAK_IF(!checkObjectExist_json(root[arrayKey], idx));
		CC_BREAK_IF(root[arrayKey][idx].IsNull());
		sRet = root[arrayKey][idx].GetString();
	} while (0);

	return sRet;
}

const rapidjson::Value &FileUtils::getDictionaryFromArray_json(const rapidjson::Value &root, const char* key, int idx)
{
	return root[key][idx];
}

bool FileUtils::checkObjectExist_json(const rapidjson::Value &root)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(root.IsNull());
		bRet = true;
	} while (0);

	return bRet;
}

bool FileUtils::checkObjectExist_json(const rapidjson::Value &root, const char* key)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(root.IsNull());
		bRet = root.HasMember(key);
	} while (0);

	return bRet;
}

bool FileUtils::checkObjectExist_json(const rapidjson::Value &root, int index)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(root.IsNull());
		CC_BREAK_IF(!root.IsArray());
		CC_BREAK_IF(index < 0 || root.Size() <= (unsigned int)index);
		bRet = true;
	} while (0);

	return bRet;
}