#pragma once

#include <string>
#include "ChunkStorage.hpp"
#include "LevelData.hpp"

class ChunkSource;
class Player;

enum StorageVersion
{
	STORAGEVERSION_UNKNOWN,
	STORAGEVERSION_LEGACY_V1,
	STORAGEVERSION_LEGACY_V2,
	STORAGEVERSION_LEGACY_V3,
	STORAGEVERSION_LEVELDB,
	STORAGEVERSION_LEVELDB_SUBCHUNKS,
	STORAGEVERSION_LEVELDB_SUBCHUNKS_RAWZIP,
	STORAGEVERSION_LEVELDB_PALETTED,
	STORAGEVERSION_LEVELDB_MULTI_TILESTORAGE
};

class LevelStorage
{
public:
	enum Status
	{
		STATUS_OPEN,
		STATUS_CORRUPTED,
		STATUS_NOT_FOUND,
		STATUS_IO_ERROR,
		STATUS_NOT_SUPPORTED,
		STATUS_INVALID_ARGUMENTS,
		STATUS_UNKNOWN
	};

	class State
	{
	public:
		Status m_status;
		std::string m_message;

	public:
		State() : m_status(STATUS_UNKNOWN) {}
		State(Status status) : m_status(status) {}
		State(Status status, const std::string& message) : m_status(status), m_message(message) {}
	};

public:
	virtual ~LevelStorage() {}
	virtual std::unique_ptr<ChunkSource> createChunkStorage(std::unique_ptr<ChunkStorage>, StorageVersion) = 0;
	virtual bool saveLevelData(LevelData*) = 0;
	virtual const std::string& getFullPath() = 0;
	virtual void savePlayerData(const std::string&, std::string&&) = 0;
	virtual void saveData(const std::string&, std::string&&) = 0;
	virtual bool isCorrupted() = 0;
	virtual std::string loadData(const std::string&) { return ""; }
	virtual State getState() = 0;
	virtual std::unique_ptr<Tag> loadPlayerData(const std::string&) = 0;
	virtual std::vector<std::string> loadAllPlayerIDs() = 0;
	virtual void save(Player&) = 0;

public:
	void savePlayerData(const std::string&, const CompoundTag&)
	{
		// TODO
	}

	void saveData(const std::string&, const CompoundTag&)
	{
		// TODO
	}

	std::unique_ptr<Tag> loadPlayerData(const Player&)
	{
		// TODO
	}
};
