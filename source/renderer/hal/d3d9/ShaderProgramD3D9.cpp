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
    : ShaderProgramD3D(shaderType, shaderSource, header, shaderPath)
{
}

ShaderProgramD3D9::~ShaderProgramD3D9()
{
}

void _translateShaderSource(std::string& source)
{
    // Dirty hack, since D3D9 doesn't support the lack of an underscore
    Util::stringReplace(source, "TEXCOORD_", "TEXCOORD");
}

DWORD _getCompilerFlags(RenderContext& context, ShaderType shaderType)
{
	DWORD compilerFlags = 0x0;

	int major, minor;
	context.getShaderLangVersion(shaderType, major, minor);

	/*
	 * Enable the use of the original Direct3D 9 HLSL compiler.
	 * OCT2006_d3dx9_31_x86.cab or OCT2006_d3dx9_31_x64.cab must be included as part of the applications redist.
	 * This flag is required to compile ps_1_x shaders without using the promotion flag to ps_2_0.
	 */
#ifdef D3DXSHADER_USE_LEGACY_D3DX9_31_DLL
	if (major == 1)
		compilerFlags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
#endif

	return compilerFlags;
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
    std::string shaderTarget = _GetShaderTarget(renderContext, m_shaderType);
	DWORD compilerFlags = _getCompilerFlags(renderContext, m_shaderType);

    HRESULT hResult;
    ComInterface<ID3DXBuffer> code, errorMsgs;
    hResult = D3DXCompileShader(
        m_shaderSource.c_str(), m_shaderSource.size(),
        NULL,
        NULL,
        "main",
        shaderTarget.c_str(),
        compilerFlags,
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

void ShaderProgramD3D9::BuildHeader(RenderContext& context, std::ostringstream& stream)
{
    stream << "#define _DIRECT3D9\n";
#ifdef _XBOX
    stream << "#define _XBOX\n";
#endif

    ShaderProgramD3D::BuildHeader(context, stream);

    // hack to fix dumb D3D9 bullshit: https://www.virtualdub.org/blog2/entry_366.html
    stream << "#define __D3D9_OFFSET_X " << (-1.0f / Minecraft::width) << "\n";
    stream << "#define __D3D9_OFFSET_Y " << (1.0f / Minecraft::height) << "\n";
}
