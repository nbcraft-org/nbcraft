#include "world/level/storage/DBStorage.hpp"
#include <leveldb/status.h>

static LevelStorageState ConvertError(const leveldb::Status& status)
{
	static const LevelStorageState levelStorageStatusMap[] = {
		LEVELSTORAGESTATUS_OPEN,              // leveldb::Status::kOk
		LEVELSTORAGESTATUS_NOT_FOUND,         // leveldb::Status::kNotFound
		LEVELSTORAGESTATUS_CORRUPTED,         // leveldb::Status::kCorruption
		LEVELSTORAGESTATUS_NOT_SUPPORTED,     // leveldb::Status::kNotSupported
		LEVELSTORAGESTATUS_INVALID_ARGUMENTS, // leveldb::Status::kInvalidArgument
		LEVELSTORAGESTATUS_IO_ERROR           // leveldb::Status::kIOError
	};

	if (status.code() > leveldb::Status::kIOError)
		return LEVELSTORAGESTATUS_UNKNOWN;

	return levelStorageStatusMap[status.code()];
}
