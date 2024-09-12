#ifndef BLESKOMAT_SPIFFS_H
#define BLESKOMAT_SPIFFS_H

#include "FS.h"
#include "SPIFFS.h"

#include <iostream>
#include <string>

namespace spiffs {
	void init();
	bool isInitialized();
	bool fileExists (const char* filePath);
	bool appendFile(const char* filePath, const char* msg);
	bool renameFile(const char* filePath, const char* newFilePath);
	bool deleteFile(const char* filePath);
}

#endif
