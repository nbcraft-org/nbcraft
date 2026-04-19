#pragma once

#include <string>
#include "renderer/hal/enums/ShaderType.hpp"

namespace mce
{
	class RenderContext;
    class ShaderProgramBase
    {
    protected:
        bool m_bValid;

    public:
        const std::string m_header;
        const std::string m_shaderPath;
        const ShaderType m_shaderType;

    public:
        ShaderProgramBase(const std::string& header, const std::string& shaderPath, ShaderType shaderType);

    public:
        bool isValid() const { return m_bValid; }

	public:
        static void SpliceShaderPath(std::string& shaderName, const std::string& shaderDir);
        static void SpliceShaderPath(std::string& shaderName);
        static void SpliceShaderExtension(std::string& shaderName, const std::string& shaderFileExtension);
        static void SpliceShaderExtension(std::string& shaderName);
        static void BuildHeader(RenderContext& context, std::ostringstream& stream);
    };
}
