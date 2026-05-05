#include <cstring>
#include <sstream>

#include "ShaderProgramOGL.hpp"

#ifdef FEATURE_GFX_SHADERS

#include "common/Util.hpp"
#include "RenderContextOGL.hpp"
#include "renderer/RenderContextImmediate.hpp"
#include "renderer/hal/ogl/helpers/ErrorHandlerOGL.hpp"
#include "renderer/platform/ogl/ShaderPrecision.hpp"

using namespace mce;

#define MULTIVERSION_STRING "// __multiversion__"

const GLenum shaderTypeMap[] = {
    /*SHADER_TYPE_VERTEX*/   GL_VERTEX_SHADER,
    /*SHADER_TYPE_FRAGMENT*/ GL_FRAGMENT_SHADER
};

ShaderProgramOGL::ShaderProgramOGL(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath)
    : ShaderProgramBase(header, shaderPath, shaderType)
{
    m_shaderName = GL_NONE;

    if (shaderSource.empty() || shaderType > SHADER_TYPE_FRAGMENT)
    {
        m_bValid = false;
        return;
    }
    
    m_bValid = compileShaderProgram(shaderSource);
}

ShaderProgramOGL::~ShaderProgramOGL()
{
    deleteShader();
}

void ShaderProgramOGL::deleteShader()
{
    xglDeleteShader(m_shaderName);
    m_shaderName = GL_NONE;
}

std::string _getVersionMacro(RenderContext& context, ShaderType shaderType)
{
	// for compatibility reasons (I've heard GLES bitches about this or something)
#if defined(__APPLE__) && TARGET_OS_IPHONE
    return "\n";
#endif

    int major, minor;
    context.getShaderLangVersion(shaderType, major, minor);

    std::string glslVersion = Util::toString(major) + Util::toString(minor);
    // make sure single-digit minor versions still end up being two digits
    if (glslVersion.size() < 3) // can be coverted to a while loop for future-proofing
        glslVersion += "0";

    if (gl::isOpenGLES3())
        glslVersion += " es";

    return "#version " + glslVersion + "\n";
}

bool ShaderProgramOGL::compileShaderProgram(std::string& shaderSource)
{
    ErrorHandlerOGL::checkForErrors();

    RenderContext& context = RenderContextImmediate::get();
    
    m_shaderName = xglCreateShader(shaderTypeMap[m_shaderType]);

    if (strncmp(shaderSource.c_str(), MULTIVERSION_STRING, sizeof(MULTIVERSION_STRING) - 1) == 0)
    {
        shaderSource.replace(0, sizeof(MULTIVERSION_STRING), _getVersionMacro(context, m_shaderType));
    }

    const GLint sourceLength = shaderSource.size();
    const GLchar* sourceStr = (const GLchar*)shaderSource.data();
    xglShaderSource(m_shaderName, 1, &sourceStr, &sourceLength);
    xglCompileShader(m_shaderName);

    GLint compileStatus;
    xglGetShaderiv(m_shaderName, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        GLint maxLength = 0;
        xglGetShaderiv(m_shaderName, GL_INFO_LOG_LENGTH, &maxLength);
        if (maxLength > 1)
        {
            std::string infoLog(maxLength, 0);
            xglGetShaderInfoLog(m_shaderName, maxLength, &maxLength, (GLchar*)infoLog.data());
            LOG_E("Compiler error:\n %s", infoLog.c_str());
        }
        return false;
    }

    ErrorHandlerOGL::checkForErrors();

    return true;
}

void ShaderProgramOGL::SpliceShaderPath(std::string& shaderName)
{
    ShaderProgramBase::SpliceShaderPath(shaderName, "/glsl");
}

void ShaderProgramOGL::BuildHeader(RenderContext& context, std::ostringstream& stream)
{
    Platform::OGL::Precision::BuildHeader(stream);
    
    const std::string& glExtensions = gl::getOpenGLExtensions();
    bool supportsSDs = (glExtensions.find("GL_OES_standard_derivatives") != std::string::npos);

    if (supportsSDs)
        stream << "#extension GL_OES_standard_derivatives : enable\n";
}

#endif // FEATURE_GFX_SHADERS
