#include "API_D3D.hpp"

#include "ShaderProgramD3D.hpp"
#include "renderer/hal/interface/RenderContext.hpp"

#include "common/Logger.hpp"
#include "common/Util.hpp"

using namespace mce;

ShaderProgramD3D::ShaderProgramD3D(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath)
    : ShaderProgramBase(header, shaderPath, shaderType)
{
    m_shaderSource = shaderSource;
}

ShaderProgramD3D::~ShaderProgramD3D()
{
}

std::string _getShaderModelType(ShaderType shaderType)
{
    switch (shaderType)
    {
    case SHADER_TYPE_VERTEX:   return "vs";
    case SHADER_TYPE_FRAGMENT: return "ps";
    case SHADER_TYPE_GEOMETRY: return "gs";

    default:
        LOG_E("Unknown shader type: %d", shaderType);
        throw std::bad_cast();
    }
}

std::string ShaderProgramD3D::_GetShaderTarget(RenderContext& context, ShaderType shaderType)
{
    std::string shaderModelType = _getShaderModelType(shaderType);
	int major, minor;
	context.getShaderLangVersion(shaderType, major, minor);

    return shaderModelType + "_" + Util::toString(major) + "_" + Util::toString(minor);
}

void _writeShaderTargetMacros(std::ostringstream& stream, int major, int minor)
{
	stream << "#define _SHADER_TARGET_MAJOR " << major << "\n";
	stream << "#define _SHADER_TARGET_MINOR " << minor << "\n";
}

void _writeShaderTargetMacros(std::ostringstream& stream, RenderContext& context, ShaderType shaderType)
{
	int major, minor;
	context.getShaderLangVersion(shaderType, major, minor);
	_writeShaderTargetMacros(stream, major, minor);
}

void ShaderProgramD3D::_WriteVersionMacros(RenderContext& context, std::ostringstream& stream)
{
	stream << "#ifdef _SHADER_TYPE_VERTEX\n";
	_writeShaderTargetMacros(stream, context, SHADER_TYPE_VERTEX);
	stream << "#elif defined(_SHADER_TYPE_FRAGMENT)\n";
	_writeShaderTargetMacros(stream, context, SHADER_TYPE_FRAGMENT);
	stream << "#endif\n";
}

void ShaderProgramD3D::SpliceShaderPath(std::string& shaderName)
{
    ShaderProgramBase::SpliceShaderPath(shaderName, "/hlsl");
}

void ShaderProgramD3D::SpliceShaderExtension(std::string& shaderName)
{
    ShaderProgramBase::SpliceShaderExtension(shaderName, ".hlsl");
}

void ShaderProgramD3D::BuildHeader(RenderContext& context, std::ostringstream& stream)
{
	_WriteVersionMacros(context, stream);
}
