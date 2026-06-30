#pragma once

#include "API_OGL.hpp"

#ifdef FEATURE_GFX_SHADERS

#include "renderer/hal/base/ShaderProgramBase.hpp"

namespace mce
{
    class ShaderProgramOGL : public ShaderProgramBase
    {
    public:
        GLuint m_shaderName;

    public:
        ShaderProgramOGL(ShaderType shaderType, std::string& shaderSource, const std::string& header, const std::string& shaderPath);
        ~ShaderProgramOGL();

    protected:
        void deleteShader();
        bool compileShaderProgram(std::string& shaderSource);

	public:
        static void SpliceShaderPath(std::string& shaderName);
        static void BuildHeader(RenderContext& context, std::ostringstream& stream);
    };
}

#endif // FEATURE_GFX_SHADERS
