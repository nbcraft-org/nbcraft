#include <cstring>
#include <sstream>

#include "API_D3D9.hpp"
#include "API_D3D9Compiler.hpp"

#include "ShaderProgramD3D9.hpp"

#include "common/Util.hpp"
#include "client/app/Minecraft.hpp"
#include "renderer/RenderContextImmediate.hpp"
#include "renderer/hal/d3d9/helpers/ErrorHandlerD3D9.hpp"

using namespace mce;

ShaderProgramD3D9::ShaderProgramD3D9(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath)
    : ShaderProgramBase(header, shaderPath, shaderType)
{
    m_shaderSource = shaderSource;
}

ShaderProgramD3D9::~ShaderProgramD3D9()
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

void _getShaderModelVersion(ShaderType shaderType, int& major, int& minor)
{
	// For the Radeon 9000
	/*switch (shaderType)
	{
	case SHADER_TYPE_VERTEX:
		major = 1;
		minor = 1;
		break;
	case SHADER_TYPE_FRAGMENT:
		major = 1;
		minor = 4;
		break;

    default:
        LOG_E("Unknown shader type: %d", shaderType);
        throw std::bad_cast();
	}*/

	// For the Xbox 360
	major = 3;
	minor = 0;
}

std::string _getShaderTarget(ShaderType shaderType)
{
    std::string shaderModelType = _getShaderModelType(shaderType);
	int major, minor;
	_getShaderModelVersion(shaderType, major, minor);

    return shaderModelType + "_" + Util::toString(major) + "_" + Util::toString(minor);
}

void _translateShaderSource(std::string& source)
{
    // Dirty hack, since D3D9 doesn't support the lack of an underscore
    Util::stringReplace(source, "TEXCOORD_", "TEXCOORD");
}

void ShaderProgramD3D9::compileShaderProgram()
{
    if (!m_shaderBytecode.empty())
        return;

    if (m_shaderSource.empty())
    {
        m_bValid = false;
        return;
    }

    _translateShaderSource(m_shaderSource);

    RenderContext& renderContext = RenderContextImmediate::get();
    std::string shaderTarget = _getShaderTarget(m_shaderType);

    HRESULT hResult;
    ComInterface<ID3DXBuffer> code, errorMsgs;
    hResult = D3DXCompileShader(
        m_shaderSource.c_str(), m_shaderSource.size(),
        NULL,
        NULL,
        "main",
        shaderTarget.c_str(),
        0x0, // D3DXSHADER_USE_LEGACY_D3DX9_31_DLL,
        *code, *errorMsgs,
        NULL);
    if (hResult != S_OK)
    {
        LPVOID errors = errorMsgs->GetBufferPointer();
        LOG_E("Error compiling shader \"%s\":", m_shaderPath.c_str());
		LOG_E((const char*)errors);
        m_bValid = false;
        return;
    }

    D3DDevice d3dDevice = renderContext.getD3DDevice();
    bool bSuccess = false;

    switch (m_shaderType)
    {
    case SHADER_TYPE_VERTEX:
        m_vertexShader.release();
        hResult = d3dDevice->CreateVertexShader((DWORD*)code->GetBufferPointer(), *m_vertexShader);
        ErrorHandlerD3D9::checkForErrors(hResult);
        bSuccess = m_vertexShader;
        break;
    case SHADER_TYPE_FRAGMENT:
        m_pixelShader.release();
        hResult = d3dDevice->CreatePixelShader((DWORD*)code->GetBufferPointer(), *m_pixelShader);
        ErrorHandlerD3D9::checkForErrors(hResult);
        bSuccess = m_pixelShader;
        break;
    case SHADER_TYPE_GEOMETRY:
        LOG_W("Geometry shaders are not supported on D3D9!");
        break;
    default:
        LOG_W("Unknown shaderType: %d", m_shaderType);
        break;
    }

    m_shaderBytecode = std::string((const char*)code->GetBufferPointer(), code->GetBufferSize());
    m_bValid = bSuccess;
}

void ShaderProgramD3D9::SpliceShaderPath(std::string& shaderName)
{
    ShaderProgramBase::SpliceShaderPath(shaderName, "/hlsl");
}

void ShaderProgramD3D9::SpliceShaderExtension(std::string& shaderName)
{
    ShaderProgramBase::SpliceShaderExtension(shaderName, ".hlsl");
}

void _writeShaderTargetMacros(std::ostringstream& stream, int major, int minor)
{
	stream << "#define _SHADER_TARGET_MAJOR " << major << "\n";
	stream << "#define _SHADER_TARGET_MINOR " << minor << "\n";
}

void _writeShaderTargetMacros(std::ostringstream& stream, ShaderType shaderType)
{
	int major, minor;
	_getShaderModelVersion(shaderType, major, minor);
	_writeShaderTargetMacros(stream, major, minor);
}

void _writeVersionMacros(RenderContext& context, std::ostringstream& stream)
{
    stream << "#define _DIRECT3D9\n";
#ifdef _XBOX
    stream << "#define _XBOX\n";
#endif

	stream << "#ifdef _SHADER_TYPE_VERTEX\n";
	_writeShaderTargetMacros(stream, SHADER_TYPE_VERTEX);
	stream << "#elif defined(_SHADER_TYPE_FRAGMENT)\n";
	_writeShaderTargetMacros(stream, SHADER_TYPE_FRAGMENT);
	stream << "#endif\n";
}

void ShaderProgramD3D9::BuildHeader(RenderContext& context, std::ostringstream& stream)
{
	_writeVersionMacros(context, stream);

    // hack to fix dumb D3D9 bullshit: https://www.virtualdub.org/blog2/entry_366.html
    stream << "#define __D3D9_OFFSET_X " << (-1.0f / Minecraft::width) << "\n";
    stream << "#define __D3D9_OFFSET_Y " << (1.0f / Minecraft::height) << "\n";
}
