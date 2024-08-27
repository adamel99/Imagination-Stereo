#include <string>
#include <OpenGL/gl.h>
#include <juce_core/juce_core.h>

class ShaderUtils
{
public:
    static GLuint loadShaderProgram(const char* vertexSource, const char* fragmentSource)
    {
        using namespace juce::gl;
        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        if (vertexShader == 0) return 0;  // Return 0 if vertex shader compilation fails

        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
        if (fragmentShader == 0) return 0;  // Return 0 if fragment shader compilation fails

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        if (!checkCompileErrors(program, "PROGRAM"))
        {
            glDeleteProgram(program);
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

private:
    static GLuint compileShader(const char* source, GLenum shaderType)
    {
        using namespace juce::gl;
        
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        if (!checkCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"))
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    static bool checkCompileErrors(GLuint shader, const std::string& type)
    {
        using namespace juce::gl;
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                juce::Logger::writeToLog("Shader Compilation Error (" + type + "): " + std::string(infoLog));
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                juce::Logger::writeToLog("Program Linking Error: " + std::string(infoLog));
                return false;
            }
        }
        return true;
    }
};
