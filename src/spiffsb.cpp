#include "spiffsb.h"

namespace {

	bool initialized = false;

	void printDirContents(const char* dirPath) {
		std::cout << "Listing directory contents: " << dirPath << std::endl;
		File root = SPIFFS.open(dirPath);
		if (!root) {
			std::cout << "ERROR: Failed to open directory" << std::endl;
			return;
		}
		if (!root.isDirectory()) {
			std::cout << "ERROR: Not a directory" << std::endl;
			return;
		}
		File file = root.openNextFile();
		while (file) {
			if (!file.isDirectory()) {
				std::cout << " " << file.name() << " (" + std::to_string(file.size()) + " bytes)" << std::endl;
			}
			file.close();
			file = root.openNextFile();
		}
		root.close();
	}
}

namespace spiffs {

	void init() {
		if (!SPIFFS.begin()) {
			std::cout << "Failed to mount SPIFFS file system" << std::endl;
			return;
		}
		initialized = true;
		std::cout << "SPIFFS file system mounted: Used " << std::to_string(SPIFFS.usedBytes()) + " of ";
		std::cout << std::to_string(SPIFFS.totalBytes()) + " available bytes." << std::endl;
		printDirContents("/");
	}

	bool isInitialized() {
		return initialized;
	}

	bool fileExists(const char* filePath) {
		if (!initialized) return false;
		return SPIFFS.exists(filePath);
	}

	bool appendFile(const char* filePath, const char* data) {
		if (!initialized) return false;
		File file = SPIFFS.open(filePath, FILE_APPEND);
		if (!file) {
			return false;
		}
		const bool result = file.print(data);
		file.close();
		return result;
	}

	bool renameFile(const char* filePath, const char* newFilePath) {
		if (!initialized) return false;
		return SPIFFS.rename(filePath, newFilePath);
	}

	bool deleteFile(const char* filePath) {
		if (!initialized) return false;
		return SPIFFS.remove(filePath);
	}
}
