#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <string>
#include "rapidjson/reader.h"
#include "rapidjson/document.h"

//using namespace rapidjson;

class FileUtils
{
public:
	FileUtils();
	~FileUtils();

	bool init();

	unsigned char* getFileData(const std::string& filename, const char* mode, size_t *size);
	bool readJson(const char *pszFileName, rapidjson::Document &doc);

	std::string stdStringForKey(const char* keyPathFormat, ...);
	int intForKey(const char* keyPathFormat, ...);
	int countForKey(const char* keyPathFormat, ...);
	double floatForKey(const char* keyPathFormat, ...);
	bool boolForKey(const char* keyPathFormat, ...);

	//bool readJson(const char *pszFileName, rapidjson::Document &doc);

	const rapidjson::Value& objectForKey(const rapidjson::Value &root, const std::string &keyPath);

	const rapidjson::Value& getSubDictionary_json(const rapidjson::Value &root, const char* key);
	const rapidjson::Value& getSubDictionary_json(const rapidjson::Value &root, const char* key, int idx);
	const rapidjson::Value& getSubDictionary_json(const rapidjson::Value &root, int idx);

	int   getIntValue_json(const rapidjson::Value& root, const char* key, int def = 0);
	float getFloatValue_json(const rapidjson::Value& root, const char* key, float def = 0.0f);
	bool  getBooleanValue_json(const rapidjson::Value& root, const char* key, bool def = false);
	const char* getStringValue_json(const rapidjson::Value& root, const char* key, const char *def = NULL);
	int   getArrayCount_json(const rapidjson::Value& root, const char* key, int def = 0);

	int   getIntValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, int def = 0);
	float getFloatValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, float def = 0.0f);
	bool  getBoolValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, bool def = false);
	const char* getStringValueFromArray_json(const rapidjson::Value& root, const char* arrayKey, int idx, const char *def = NULL);
	const rapidjson::Value &getDictionaryFromArray_json(const rapidjson::Value &root, const char* key, int idx);
	bool checkObjectExist_json(const rapidjson::Value &root);
	bool checkObjectExist_json(const rapidjson::Value &root, const char* key);
	bool checkObjectExist_json(const rapidjson::Value &root, int index);

protected:
	rapidjson::Document m_docData;

};



#endif // !_FILEUTILS_H_

