#include "MinecartModel.hpp"
#include "common/Utils.hpp"

MinecartModel::MinecartModel() : Model(64, 32)
{
    m_cubes[0] = new ModelPart(this, 0, 10);
    m_cubes[1] = new ModelPart(this, 0, 0);
    m_cubes[2] = new ModelPart(this, 0, 0);
    m_cubes[3] = new ModelPart(this, 0, 0);
    m_cubes[4] = new ModelPart(this, 0, 0);
    m_cubes[5] = new ModelPart(this, 44, 10);
    constexpr int w = 20;
    constexpr int d = 8;
    constexpr int h = 16;
    constexpr int yOff = 4;
    m_cubes[0]->addBox((-w / 2), (-h / 2), -1.0f, w, h, 2, 0.0f);
    m_cubes[0]->setPos(0.0f, (0 + yOff), 0.0f);
    m_cubes[5]->addBox((-w / 2 + 1), (-h / 2 + 1), -1.0f, w - 2, h - 2, 1, 0.0f);
    m_cubes[5]->setPos(0.0f, (0 + yOff), 0.0f);
    m_cubes[1]->addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_cubes[1]->setPos((-w / 2 + 1), (0 + yOff), 0.0f);
    m_cubes[2]->addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_cubes[2]->setPos((w / 2 - 1), (0 + yOff), 0.0f);
    m_cubes[3]->addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_cubes[3]->setPos(0.0f, (0 + yOff), (-h / 2 + 1));
    m_cubes[4]->addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_cubes[4]->setPos(0.0f, (0 + yOff), (h / 2 - 1));
    m_cubes[0]->m_rot.x = M_PI_2;
    m_cubes[1]->m_rot.y = M_PI + M_PI_2;
    m_cubes[2]->m_rot.y = M_PI_2;
    m_cubes[3]->m_rot.y = M_PI;
    m_cubes[5]->m_rot.x = -M_PI_2;
}

MinecartModel::~MinecartModel()
{
    for (int i = 0; i < 6; ++i)
    {
        delete m_cubes[i];
    }
}

void MinecartModel::render(float time, float r, float bob, float yRot, float xRot, float scale)
{
    m_cubes[5]->m_pos.y = 4.0f - bob;

    for (int i = 0; i < 6; ++i)
    {
        m_cubes[i]->render(scale);
    }
}
