#pragma once

namespace mce
{
	struct ViewportOrigin
	{
		static const ViewportOrigin ZERO;

		int leftX;
		int bottomLeftY; // OpenGL's default
		int topLeftY; // Direct3D's default

		ViewportOrigin()
		{
			leftX = 0;
			bottomLeftY = 0;
			topLeftY = 0;
		}
	};
}
