#pragma once

#include "MobRenderer.hpp"
#include "world/entity/Slime.hpp"

class SlimeRenderer : public MobRenderer
{
public:
	SlimeRenderer(Model*, Model*, float);
	~SlimeRenderer();
protected:
	void render(const Entity& entity, const Vec3& pos, float rot, float a) override;
	int prepareArmor(const Mob& spider, int layer, float a) override;
	void scale(const Mob& mob, Matrix& matrix, float a) override;
private:
	Model* m_armor;
};
