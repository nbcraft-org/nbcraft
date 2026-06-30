#include "world/level/storage/DBStorage.hpp"
#include <leveldb/status.h>

static LevelStorage::Status ConvertError(const leveldb::Status& status)
{
	static const LevelStorage::Status levelStorageStatusMap[] = {
		LevelStorage::STATUS_OPEN,              // leveldb::Status::kOk
		LevelStorage::STATUS_NOT_FOUND,         // leveldb::Status::kNotFound
		LevelStorage::STATUS_CORRUPTED,         // leveldb::Status::kCorruption
		LevelStorage::STATUS_NOT_SUPPORTED,     // leveldb::Status::kNotSupported
		LevelStorage::STATUS_INVALID_ARGUMENTS, // leveldb::Status::kInvalidArgument
		LevelStorage::STATUS_IO_ERROR           // leveldb::Status::kIOError
	};

	if (status.code() > leveldb::Status::kIOError)
		return LevelStorage::STATUS_UNKNOWN;

	return levelStorageStatusMap[status.code()];
}
