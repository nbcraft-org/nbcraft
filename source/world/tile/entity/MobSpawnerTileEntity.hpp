#pragma once

#include <string>
#include "TileEntity.hpp"
#include "world/entity/EntityTypeDescriptor.hpp"

class MobSpawnerTileEntity : public TileEntity
{
public:
    MobSpawnerTileEntity();

    const std::string& getEntityId() const { return m_pEntityDescriptor->getEntityType().getName(); }

    void setEntityId(const std::string& id)
    {
        const EntityTypeDescriptor* descriptor = EntityTypeDescriptor::GetByEntityTypeName(id);
        if (descriptor)
            m_pEntityDescriptor = descriptor;
    }

    bool isNearPlayer() const;
    void tick() override;
    void delay();

    void load(const CompoundTag& tag) override;

    void save(CompoundTag& tag) const override;

public:
    static int MAX_DIST;

    int m_spawnDelay;
    const EntityTypeDescriptor* m_pEntityDescriptor;
    float m_spin;
    float m_oSpin;
};
