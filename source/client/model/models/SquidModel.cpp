/********************************************************************
    Minecraft: Pocket Edition - Decompilation Project
    Copyright (C) 2023 iProgramInCpp

    The following code is licensed under the BSD 1 clause license.
    SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
// copied from wilyicaro's PEtoLE
#include "SquidModel.hpp"
#include "common/Mth.hpp"

SquidModel::SquidModel() :
    Model(64, 32),
    m_body(0, 0)
{
    m_body.setModel(this);

    int yoffs = -16;
    m_body.addBox(-6.0F, -8.0F, -6.0F, 12, 16, 12);
    m_body.m_pos.y += (float)(24 + yoffs);

    for (int i = 0; i < 8; ++i)
    {
        m_tentacles[i] = new ModelPart(48, 0);
        m_tentacles[i]->setModel(this);
        float angle = i * M_PI * 2.0 / 8;
        float xo = Mth::cos(angle) * 5.0F;
        float yo = Mth::sin(angle) * 5.0F;
        m_tentacles[i]->addBox(-1.0F, 0.0F, -1.0F, 2, 18, 2);
        m_tentacles[i]->m_pos.x = xo;
        m_tentacles[i]->m_pos.z = yo;
        m_tentacles[i]->m_pos.y = (31 + yoffs);
        angle = i * M_PI * -2.0 / 8 + M_PI_2;
        m_tentacles[i]->m_rot.y = angle;
    }
}

SquidModel::~SquidModel()
{
    //for (int i = 0; i < 8; ++i) {
        //delete m_tentacles[i];
    //}
}

void SquidModel::render(float time, float r, float bob, float yRot, float xRot, float scale)
{
    setupAnim(time, r, bob, yRot, xRot, scale);

    m_body.render(scale);

    for (int i = 0; i < 8; ++i) {
        m_tentacles[i]->render(scale);
    }
}

void SquidModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale)
{
    for (int i = 0; i < 8; ++i) {
        m_tentacles[i]->m_rot.x = bob;
    }
}