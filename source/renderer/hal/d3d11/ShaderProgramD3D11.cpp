#include <cstring>
#include <sstream>

#include "API_D3D11.hpp"
#include "API_D3D11Compiler.hpp"

#include "ShaderProgramD3D11.hpp"

#include "common/Util.hpp"
#include "renderer/RenderContextImmediate.hpp"
#include "renderer/hal/dxgi/helpers/ErrorHandlerDXGI.hpp"

using namespace mce;

ShaderProgramD3D11::ShaderProgramD3D11(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath)
    : ShaderProgramD3D(shaderType, shaderSource, header, shaderPath)
{
}

ShaderProgramD3D11::~ShaderProgramD3D11()
{
}

void ShaderProgramD3D11::compileShaderProgram()
{
    if (!m_shaderBytecode.empty())
        return;

    if (m_shaderSource.empty())
    {
        m_bValid = false;
        return;
    }

    RenderContext& renderContext = RenderContextImmediate::get();
    std::string shaderTarget = _GetShaderTarget(renderContext, m_shaderType);

    HRESULT hResult;
    ComInterface<ID3DBlob> code, errorMsgs;
    hResult = D3DCompile(
        m_shaderSource.c_str(), m_shaderSource.size(),
        m_shaderPath.c_str(),
        NULL,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        shaderTarget.c_str(),
        D3DCOMPILE_OPTIMIZATION_LEVEL3, 0x0,
        *code, *errorMsgs
    );
    if (hResult != S_OK)
    {
        LPVOID errors = errorMsgs->GetBufferPointer();
        LOG_E("Error compiling shader: %s", errors);
        m_bValid = false;
        return;
    }

    D3DDevice d3dDevice = renderContext.getD3DDevice();
    bool bSuccess = false;

    switch (m_shaderType)
    {
    case SHADER_TYPE_VERTEX:
        m_vertexShader.release();
        hResult = d3dDevice->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, *m_vertexShader);
        ErrorHandlerDXGI::checkForErrors(hResult);
        bSuccess = m_vertexShader;
        break;
    case SHADER_TYPE_FRAGMENT:
        m_pixelShader.release();
        hResult = d3dDevice->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, *m_pixelShader);
        ErrorHandlerDXGI::checkForErrors(hResult);
        bSuccess = m_pixelShader;
        break;
    case SHADER_TYPE_GEOMETRY:
        m_geometryShader.release();
        hResult = d3dDevice->CreateGeometryShader(code->GetBufferPointer(), code->GetBufferSize(), NULL, *m_geometryShader);
        ErrorHandlerDXGI::checkForErrors(hResult);
        bSuccess = m_geometryShader;
        break;
    default:
        LOG_W("Unknown shaderType: %d", m_shaderType);
        break;
    }

    m_shaderBytecode = std::string((const char*)code->GetBufferPointer(), code->GetBufferSize());
    m_bValid = bSuccess;
}

std::string _getShaderModelVersionSuffix(D3D_FEATURE_LEVEL featureLevel)
{
    switch (featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_1:
    case D3D_FEATURE_LEVEL_11_0:
    case D3D_FEATURE_LEVEL_10_1:
    case D3D_FEATURE_LEVEL_10_0:
        return "";

    case D3D_FEATURE_LEVEL_9_3:
        return "_level_9_3";

    case D3D_FEATURE_LEVEL_9_2:
    case D3D_FEATURE_LEVEL_9_1:
        return "_level_9_1";

    default:
        return "";
    }
}

std::string ShaderProgramD3D11::_GetShaderTarget(RenderContext& context, ShaderType shaderType)
{
    std::string shaderTarget = ShaderProgramD3D::_GetShaderTarget(context, shaderType);

    D3D_FEATURE_LEVEL featureLevel = context.m_featureLevel;
    std::string shaderModelSuffix = _getShaderModelVersionSuffix(featureLevel);

    return shaderTarget + shaderModelSuffix;
}

void ShaderProgramD3D11::BuildHeader(RenderContext& context, std::ostringstream& stream)
{
    ShaderProgramD3D::BuildHeader(context, stream);

    if (!context.supportsR8G8B8A8_SNORM())
        stream << "#define R8G8B8A8_SNORM_UNSUPPORTED";
}
