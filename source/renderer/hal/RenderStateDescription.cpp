#include "RenderStateDescription.hpp"

using namespace mce;

RenderStateDescription::RenderStateDescription()
{
	enableTexture = false;
	rescaleNormals = false;
}

bool RenderStateDescription::operator==(const RenderStateDescription& other) const
{
	return enableTexture == other.enableTexture
		&& rescaleNormals == other.rescaleNormals;
}

bool RenderStateDescription::operator!=(const RenderStateDescription& other) const
{
	return !(*this == other);
}
