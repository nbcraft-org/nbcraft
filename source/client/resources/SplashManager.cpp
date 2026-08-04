#include <sstream>
#include "SplashManager.hpp"
#include "Resource.hpp"
#include "common/Util.hpp"

SplashManager* SplashManager::instance = nullptr;

std::string _getGFXAPIVersionString()
{
#if MCE_GFX_API_OGL
    const gl::Version& glVersion = gl::Version::singleton();
    const char* glVersionString;
    if (glVersion.gles)
    {
        glVersionString = "OpenGL ES %d.%d!";
    }
    else
    {
        glVersionString = "OpenGL %d.%d!";
    }

    return Util::format(glVersionString, glVersion.major, glVersion.minor);
#elif MCE_GFX_API_D3D11
    return "Direct3D 11.1!";
#elif MCE_GFX_API_D3D9
    return "Direct3D 9!";
#endif
}

void SplashManager::init(const std::string& user)
{
    m_splashes.clear();
    std::string file;
    if (!Resource::load("title/splashes.txt", file)) return;
    std::istringstream stream(file);
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty())
            continue;

        if (line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        if (line.size() >= 2 && line[0] == '/' && line[1] == '/')
            continue;

        m_splashes.push_back(line);
    }

    m_splashes.push_back(_getGFXAPIVersionString());

    m_isYouSplash = Util::format("%s IS YOU", user.c_str());
}

const std::string& SplashManager::getSplash()
{
    //Add splashes for holidays?
    if (m_random.nextInt(m_splashes.size()) == 42) return m_isYouSplash;

    return m_splashes[m_random.nextInt(m_splashes.size())];
}
