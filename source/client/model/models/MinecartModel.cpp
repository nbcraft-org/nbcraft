#include "MinecartModel.hpp"
#include "common/Utils.hpp"

MinecartModel::MinecartModel() : Model(64, 32),
    m_outerFloor(this, 0, 10),
    m_frontBarrier(this, 0, 0),
    m_backBarrier(this, 0, 0),
    m_leftBarrier(this, 0, 0),
    m_rightBarrier(this, 0, 0),
    m_innerFloor(this, 44, 10)
{
    constexpr int w = 20;
    constexpr int d = 8;
    constexpr int h = 16;
    constexpr int yOff = 4;

    m_outerFloor.addBox((-w / 2), (-h / 2), -1.0f, w, h, 2, 0.0f);
    m_outerFloor.setPos(0.0f, (0 + yOff), 0.0f);
    m_innerFloor.addBox((-w / 2 + 1), (-h / 2 + 1), -1.0f, w - 2, h - 2, 1, 0.0f);
    m_innerFloor.setPos(0.0f, (0 + yOff), 0.0f);
    m_frontBarrier.addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_frontBarrier.setPos((-w / 2 + 1), (0 + yOff), 0.0f);
    m_backBarrier.addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_backBarrier.setPos((w / 2 - 1), (0 + yOff), 0.0f);
    m_leftBarrier.addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_leftBarrier.setPos(0.0f, (0 + yOff), (-h / 2 + 1));
    m_rightBarrier.addBox((-w / 2 + 2), (-d - 1), -1.0f, w - 4, d, 2, 0.0f);
    m_rightBarrier.setPos(0.0f, (0 + yOff), (h / 2 - 1));
    m_outerFloor.m_rot.x = M_PI_2;
    m_frontBarrier.m_rot.y = M_PI + M_PI_2;
    m_backBarrier.m_rot.y = M_PI_2;
    m_leftBarrier.m_rot.y = M_PI;
    m_innerFloor.m_rot.x = -M_PI_2;
}

void MinecartModel::onGraphicsReset()
{
    m_outerFloor.m_bCompiled = false;
    m_frontBarrier.m_bCompiled = false;
    m_backBarrier.m_bCompiled = false;
    m_leftBarrier.m_bCompiled = false;
    m_rightBarrier.m_bCompiled = false;
    m_innerFloor.m_bCompiled = false;
}

void MinecartModel::render(float time, float r, float bob, float yRot, float xRot, float scale)
{
    m_innerFloor.m_pos.y = 4.0f - bob;

    m_outerFloor.render(scale);
    m_frontBarrier.render(scale);
    m_backBarrier.render(scale);
    m_leftBarrier.render(scale);
    m_rightBarrier.render(scale);
    m_innerFloor.render(scale);
}
