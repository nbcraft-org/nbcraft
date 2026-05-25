#include "ShaderProgramBase.hpp"

using namespace mce;

ShaderProgramBase::ShaderProgramBase(const std::string& header, const std::string& shaderPath, ShaderType shaderType)
    : m_header(header), m_shaderPath(shaderPath), m_shaderType(shaderType)
{
    m_bValid = false;
}

void ShaderProgramBase::SpliceShaderPath(std::string& shaderName, const std::string& shaderDir)
{
    size_t shaderPathPos = shaderName.find_first_not_of("shaders");
    if (shaderPathPos != std::string::npos && shaderName.find(shaderDir) == std::string::npos)
    {
        shaderName.insert(shaderPathPos, shaderDir);
    }
}

void ShaderProgramBase::SpliceShaderPath(std::string& shaderName)
{
}

void ShaderProgramBase::SpliceShaderExtension(std::string& shaderName, const std::string& shaderFileExtension)
{
    if (shaderName.empty())
        return;

    const size_t extensionSize = shaderFileExtension.size();
    if (shaderName.size() <= extensionSize || shaderName.substr(shaderName.length() - extensionSize, extensionSize) != shaderFileExtension)
        shaderName.append(shaderFileExtension);
}

void ShaderProgramBase::SpliceShaderExtension(std::string& shaderName)
{
}

void ShaderProgramBase::BuildHeader(RenderContext& context, std::ostringstream& stream)
{
}
