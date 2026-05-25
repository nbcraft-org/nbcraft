#pragma once

#include <string>
#include <vector>
#include <map>

class Level;
class TileEntity;
class TileEntityType;

class TileEntityFactory
{
private:
	static std::map<std::string, TileEntityType*> _types;

public:
	static void initTileEntities();
	static void teardownTileEntities();
	static const TileEntityType* getType(const std::string& name) { return _types[name]; }

public:
	template <typename T>
	static TileEntityType* registerTileEntity(const std::string& name);
};

class TileEntityType
{
public:
	static TileEntityType* furnace;
	static TileEntityType* chest;
	static TileEntityType* noteblock;

public:
	friend class TileEntityFactory;
	typedef TileEntity* (*CreateFunction)();

public:
	TileEntityType(const std::string& name, CreateFunction func)
		: m_name(name)
		, m_function(func)
	{
	}

public:
	const std::string& getName() const { return m_name; }
	TileEntity* newTileEntity() const { return m_function(); }

private:
	std::string m_name;
	CreateFunction m_function;

	template <typename T>
	static TileEntity* CreateType() { return new T(); }
};

template <typename T>
TileEntityType* TileEntityFactory::registerTileEntity(const std::string& name)
{
	TileEntityType* type = new TileEntityType(name, &TileEntityType::CreateType<T>);
	_types[type->m_name] = type;
	return type;
}