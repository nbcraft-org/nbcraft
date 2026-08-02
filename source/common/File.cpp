#include "common/File.hpp"
#include "common/Util.hpp"
#include "common/Utils.hpp"
#include <fstream>

static bool CreateOneFolder(const std::string& directoryPath)
{
	if (File::exists(directoryPath))
		return true;

	return XPL_MKDIR(directoryPath.c_str(), 0755) == 0;
}

std::string File::cleanPath(const std::string& path)
{
	std::string cleanedPath;
	bool lastCharWasSlash = false;

	for (int i = 0; i < path.size(); ++i)
	{
		char c = path[i];
		if (c == '\\' || c == '/')
		{
			lastCharWasSlash = true;
		}
		else
		{
			if (lastCharWasSlash)
			{
				cleanedPath += '/';
				lastCharWasSlash = false;
			}

			cleanedPath += c;
		}
	}

	return cleanedPath;
}

std::vector<std::string> File::splitPath(const std::string& path)
{
	std::string cleanedPath = cleanPath(path);

	std::vector<std::string> components;
	std::string currentComponent;

	for (int i = 0; i < cleanedPath.size(); ++i)
	{
		char c = cleanedPath[i];
		if (c == '/')
		{
			components.push_back(currentComponent);
			currentComponent.clear();
		}
		else
		{
			currentComponent += c;
		}
	}

	if (!currentComponent.empty())
		components.push_back(currentComponent);

	return components;
}

bool File::exists(const std::string& path)
{
	return XPL_ACCESS(path.c_str(), F_OK) == 0;
}

static void CopyCallback(const std::string& sourcePath, const std::string& destinationPath, const std::string& path, const struct stat& fileStat)
{
	std::string name = path.substr(sourcePath.length());
	std::string newPath = destinationPath + name;

	if (!File::isDirectory(fileStat))
	{
		File::createFolder(Util::getDirectory(newPath));

		std::ifstream fromSource(path, std::ios::binary);
		std::ofstream toSource(newPath, std::ios::binary);
		toSource << fromSource.rdbuf();
	}
}

File::Result File::copy(const std::string& sourcePath, const std::string& destinationPath)
{
	if (sourcePath.empty() || destinationPath.empty())
		return RESULT_BAD_PATH;
	
	// make sure paths are ok
	std::string cleanSourcePath = cleanPath(sourcePath);
	std::string cleanDestinationPath = cleanPath(destinationPath);
	if (cleanSourcePath[cleanSourcePath.length() - 1] != '/')
		cleanSourcePath += '/';
	if (cleanDestinationPath[cleanDestinationPath.length() - 1] != '/')
		cleanDestinationPath += '/';

	forEachIn(sourcePath, true, std::bind(CopyCallback, sourcePath, destinationPath, std::placeholders::_1, std::placeholders::_2));

	return RESULT_OK;
}

bool File::isDirectory(const struct stat& fileStat)
{
	return S_ISDIR(fileStat.st_mode);
}

bool File::createFolder(const std::string& path)
{
	std::vector<std::string> pathComponents = splitPath(path);
	if (pathComponents.empty())
		return true;

	std::vector<std::string>::const_iterator it;
	for (it = pathComponents.end() - 1; it != pathComponents.begin(); it--)
	{
		if (File::exists(*it))
		{
			it++;
			break;
		}
	}

	for (; it != pathComponents.end(); it++)
	{
		if (!CreateOneFolder(*it))
			return false;
	}

	return true;
}

bool File::deleteEmptyDirectory(const std::string& path)
{
	return _rmdir(path.c_str()) == 0;
}

static void GetSizeCallback(uint64_t& size, const std::string& path, const struct stat& fileStat)
{
	if (!File::isDirectory(fileStat))
		size += fileStat.st_size;
}

uint64_t File::getSize(const std::string& path)
{
	uint64_t size = 0;
	forEachIn(path, true, std::bind(GetSizeCallback, size, std::placeholders::_1, std::placeholders::_2));
	return size;
}

long File::getRemainingSize(FILE* stream)
{
	if (!stream)
		return 0;

	long currentPos = ftell(stream);
	fseek(stream, 0, SEEK_END);
	long fileSize = ftell(stream);
	fseek(stream, currentPos, SEEK_SET);
	return fileSize - currentPos;
}

File::Result File::forEachIn(const std::string& directory, bool recurseDirectories, const std::function<void(const std::string& path, const struct stat& fileStat)>& callback)
{
	if (directory.empty())
		return RESULT_BAD_PATH;

	std::string cleanDirectory = cleanPath(directory);
	if (cleanDirectory[cleanDirectory.length() - 1] != '/')
		cleanDirectory += '/';

	DIR* dir = opendir(cleanDirectory.c_str());
	if (!dir)
		return RESULT_NOT_FOUND;

	while (dirent* file = readdir(dir))
	{
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
			continue;

		std::string path = cleanDirectory;
		//path += '/';
		path += file->d_name;

		struct stat fileStat;
		stat(path.c_str(), &fileStat);

		if (recurseDirectories)
		{
			if (isDirectory(fileStat))
				forEachIn(path, true, callback);
		}

		callback(path, fileStat);
	}

	closedir(dir);
	return RESULT_OK;
}
