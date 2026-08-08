#pragma once

#include "MobRenderer.hpp"
#include "world/entity/Slime.hpp"

class SlimeRenderer : public MobRenderer
{
public:
	SlimeRenderer(Model*, Model*, float);
	~SlimeRenderer();

protected:
	bool prepareArmor(const Mob& spider, int layer, float a) override;
	void scale(const Mob& mob, Matrix& matrix, float a) override;

private:
	Model* m_armor;
};
