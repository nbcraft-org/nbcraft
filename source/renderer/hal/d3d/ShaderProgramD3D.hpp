#pragma once

#include <sstream>

#include "API_D3D.hpp"

#include "renderer/hal/base/ShaderProgramBase.hpp"

namespace mce
{
    class ShaderProgramD3D : public ShaderProgramBase
    {
    public:
        std::string m_shaderBytecode;
        std::string m_shaderSource;

    public:
        ShaderProgramD3D(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath);
        ~ShaderProgramD3D();

    public:
        void compileShaderProgram();

    protected:
        static std::string _GetShaderTarget(RenderContext& context, ShaderType shaderType);
        static void _WriteVersionMacros(RenderContext& context, std::ostringstream& stream);

	public:
        static void SpliceShaderPath(std::string& shaderName);
        static void SpliceShaderExtension(std::string& shaderName);
        static void BuildHeader(RenderContext& context, std::ostringstream& stream);
    };
}
