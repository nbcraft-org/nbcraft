#include "leveldb/status.h"


enum LevelStorageState
{
	LEVELSTORAGESTATUS_OPEN,
	LEVELSTORAGESTATUS_CORRUPTED,
	LEVELSTORAGESTATUS_NOT_FOUND,
	LEVELSTORAGESTATUS_IO_ERROR,
	LEVELSTORAGESTATUS_NOT_SUPPORTED,
	LEVELSTORAGESTATUS_INVALID_ARGUMENTS,
	LEVELSTORAGESTATUS_UNKNOWN
};


static LevelStorageState ConvertError(const leveldb::Status& status)
{
	static const LevelStorageState levelStorageStatusMap[6] = {
		LEVELSTORAGESTATUS_OPEN,              // leveldb::Status::kOk
		LEVELSTORAGESTATUS_NOT_FOUND,         // leveldb::Status::kNotFound
		LEVELSTORAGESTATUS_CORRUPTED,         // leveldb::Status::kCorruption
		LEVELSTORAGESTATUS_NOT_SUPPORTED,     // leveldb::Status::kNotSupported
		LEVELSTORAGESTATUS_INVALID_ARGUMENTS, // leveldb::Status::kInvalidArgument
		LEVELSTORAGESTATUS_IO_ERROR           // leveldb::Status::kIOError
	};

	return levelStorageStatusMap[status.code()];
}
