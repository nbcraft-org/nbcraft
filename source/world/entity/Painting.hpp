#pragma once

#include "Entity.hpp"
#include "PaintingType.hpp"

class Painting : public Entity
{
private:
	void _init();
	float _offs(int);

	int m_checkInterval;

public:
	Painting(Level*);
	Painting(Level*, const TilePos& pos, int);
	Painting(Level*, const TilePos& pos, int, std::string motive);

	PaintingType& getPaintingType() const
	{
		return PaintingType::types[m_paintingType];
	}

	void setDir(int face);

	void tick() override;

	bool survives();

	void addAdditionalSaveData(CompoundTag& tag) const override;

	void readAdditionalSaveData(const CompoundTag& tag) override;

	bool hurt(Entity*, int) override;

	bool isPickable() const override { return true; }

	int m_dir;
	TilePos m_tilePos;
	int m_paintingType;
};
