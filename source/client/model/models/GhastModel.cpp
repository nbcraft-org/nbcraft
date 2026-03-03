#include "GhastModel.hpp"
#include "common/Mth.hpp"
#include "common/Random.hpp"

GhastModel::GhastModel() :
    Model(64, 32),
    m_body(this, 0, 0)
{
    //m_body.setModel(this);

    int yoffs = -16;
    m_body.addBox(-8.0F, -8.0F, -8.0F, 16, 16, 16);
    m_body.m_pos.y += float(24 + yoffs);
    Random* random = new Random(1660L);

    for (int i = 0; i < 9; ++i)
    {
        m_tentacles[i] = new ModelPart(this, 48, 0);
        //m_tentacles[i]->setModel(this);
        float xo = ((float(i % 3) - float(i / 3 % 2) * 0.5F + 0.25F) / 2.0F * 2.0F - 1.0F) * 5.0F;
        float yo = (float(i / 3) / 2.0F * 2.0F - 1.0F) * 5.0F;
        int len = random->nextInt(7) + 8;
        m_tentacles[i]->addBox(-1.0F, 0.0F, -1.0F, 2, len, 2);
        m_tentacles[i]->m_pos.x = xo;
        m_tentacles[i]->m_pos.z = yo;
        m_tentacles[i]->m_pos.y = float(31 + yoffs);
    }
}

GhastModel::~GhastModel()
{
    for (int i = 0; i < 9; ++i) {
        delete m_tentacles[i];
    }
}

void GhastModel::render(float time, float r, float bob, float yRot, float xRot, float scale)
{
    setupAnim(time, r, bob, yRot, xRot, scale);

    m_body.render(scale);

    for (int i = 0; i < 9; ++i) {
        m_tentacles[i]->render(scale);
    }
}

void GhastModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale)
{
    for (int i = 0; i < 9; ++i) {
        m_tentacles[i]->m_rot.x = 0.2F * Mth::sin(bob * 0.3F + float(i)) + 0.4F;
    }
}