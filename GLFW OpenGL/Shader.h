#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //ensures ifstream objects can trow exceptions:
        vShaderFile.exceptions ( std::ifstream::badbit);
        fShaderFile.exceptions ( std::ifstream::badbit);
        try
        {
            //Opens files
            vShaderFile.open ( vertexPath );
            fShaderFile.open ( fragmentPath );
            std::stringstream vShaderStream, fShaderStream;
            //Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            //Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        
        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();
        //2. Compile shaders
        GLuint vertexShader, fragmentShader;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource (vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        //Print compile errors if any
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(! success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR""SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        //Fragment Shader
        fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER);
        glShaderSource( fragmentShader, 1, &fShaderCode, NULL );
        glCompileShader( fragmentShader);
        //Print compile errors if any
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        //Shader Program
        this->Program = glCreateProgram();
        glAttachShader( this->Program, vertexShader);
        glAttachShader( this->Program, fragmentShader);
        glLinkProgram( this->Program);
        //Print linking errors if any
        glGetProgramiv( this->Program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader( vertexShader);
        glDeleteShader( fragmentShader);
        
    }

    //Use the current shader
    void Use()
    {
        glUseProgram( this->Program);
    }
};

#endif /* SHADER_H */
