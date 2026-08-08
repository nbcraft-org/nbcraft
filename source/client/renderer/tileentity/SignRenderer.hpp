#pragma once

#include "TileEntityRenderer.hpp"
#include "world/tile/entity/SignTileEntity.hpp"
#include "client/model/models/SignModel.hpp"

class SignRenderer : public TileEntityRenderer<SignTileEntity>
{
protected:
	class Materials
	{
	public:
		mce::MaterialPtr sign_text;

		Materials();
	};

public:
    void renderTyped(SignTileEntity* spawner, const Vec3&, float partialTicks) override;

    SignModel m_signModel;
	Materials m_materials;
};
