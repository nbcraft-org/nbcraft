#pragma once
#include <string>
#include <vector>
#include <functional>

struct stat;

class File
{
public:
	enum Result
	{
		RESULT_OK,
		RESULT_NOT_FOUND,
		RESULT_BAD_PATH
	};

public:
	static std::string cleanPath(const std::string& path);
	static std::vector<std::string> splitPath(const std::string& path);

	static bool exists(const std::string& path);

	static bool isDirectory(const struct stat& fileStat);
	static bool createFolder(const std::string& path);
	static bool deleteEmptyDirectory(const std::string& path);

	static Result copy(const std::string& sourcePath, const std::string& destinationPath);

	static uint64_t getSize(const std::string& path);
	static long getRemainingSize(FILE* stream);

	static Result forEachIn(const std::string& directory, bool recurseDirectories, const std::function<void(const std::string& path, const struct stat& fileStat)>& callback);
};
