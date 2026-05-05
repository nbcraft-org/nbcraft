#pragma once

#include "API_D3D9.hpp"

#include "renderer/hal/d3d/ShaderProgramD3D.hpp"
#include "thirdparty/com/ComInterface.hpp"

namespace mce
{
    class ShaderProgramD3D9 : public ShaderProgramD3D
    {
    public:
        ComInterface<IDirect3DVertexShader9> m_vertexShader;
        ComInterface<IDirect3DPixelShader9> m_pixelShader;

    public:
        ShaderProgramD3D9(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath);
        ~ShaderProgramD3D9();

    public:
        void compileShaderProgram();

	public:
        static void BuildHeader(RenderContext& context, std::ostringstream& stream);
    };
}
