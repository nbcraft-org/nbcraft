#pragma once

#include "API_D3D11.hpp"

#include "renderer/hal/d3d/ShaderProgramD3D.hpp"
#include "thirdparty/com/ComInterface.hpp"

namespace mce
{
    class ShaderProgramD3D11 : public ShaderProgramD3D
    {
    public:
        ComInterface<ID3D11VertexShader> m_vertexShader;
        ComInterface<ID3D11PixelShader> m_pixelShader;
        ComInterface<ID3D11GeometryShader> m_geometryShader;

    public:
        ShaderProgramD3D11(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath);
        ~ShaderProgramD3D11();

    public:
        void compileShaderProgram();

    protected:
        std::string _GetShaderTarget(RenderContext& context, ShaderType shaderType);

	public:
        static void BuildHeader(RenderContext& context, std::ostringstream& stream);
    };
}
