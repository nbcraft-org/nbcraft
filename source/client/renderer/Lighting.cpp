#include "Lighting.hpp"
#include "renderer/RenderContextImmediate.hpp"

void Lighting::turnOff(bool teardown)
{
	mce::RenderContext& renderContext = mce::RenderContextImmediate::get();
	renderContext.disableFixedLighting(teardown);
}

void Lighting::turnOn(bool init)
{
	mce::RenderContext& renderContext = mce::RenderContextImmediate::get();
	renderContext.enableFixedLighting(init);
	renderContext.setShadeMode(mce::SHADE_MODE_FLAT);
}

void Lighting::turnOn(const Matrix& matrix)
{
	mce::RenderContext& renderContext = mce::RenderContextImmediate::get();
	renderContext.loadMatrix(MATRIX_VIEW, matrix); // matrix is loaded purely for use by glLight calls
	turnOn();
}

void Lighting::turnOnItems()
{
	MatrixStack::Ref matrix = MatrixStack::World.push();
	matrix->rotate(-30.0f, Vec3::UNIT_Y);
	matrix->rotate(165.0f, Vec3::UNIT_X);
	turnOn(matrix);
}

void Lighting::turnOnConsoleUiItems()
{
	MatrixStack::Ref matrix = MatrixStack::World.push();
#ifdef ENH_NEWER_CONSOLE_ITEM_LIGHTING
	matrix->rotate(155.0f, Vec3::UNIT_X);
#else
	matrix->rotate(20.0f, Vec3::UNIT_Y);
	matrix->rotate(145.0f, Vec3::UNIT_X);
#endif
	turnOn(matrix);
}
