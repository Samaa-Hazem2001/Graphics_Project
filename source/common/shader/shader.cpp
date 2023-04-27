#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();
 

   /* 
     glCreateShader — Creates a shader object
     C Specification
         GLuint glCreateShader(	GLenum shaderType);
     Parameters
       shaderType : Specifies the type of shader to be created. Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
   */
    GLuint shaderID = glCreateShader(type);

    // send the source code to the shader and compile it

    /*
     glShaderSource — Replaces the source code in a shader object

     C Specification
            void glShaderSource(	GLuint shader,
                GLsizei count,
                const GLchar **string,
                const GLint *length);
 
     Parameters
        1-shader : Specifies the handle of the shader object whose source code is to be replaced.

        2-count : Specifies the number of elements in the string and length arrays.

        3-string : Specifies an array of pointers to strings containing the source code to be loaded into the shader.

        4-length : Specifies an array of string lengths.

    */

   /*
   Name
glCompileShader — Compiles a shader object

C Specification
 void glCompileShader(	GLuint shader);
 
Parameters
1-shader : Specifies the shader object to be compiled.

Description
 glCompileShader compiles the source code strings that have been stored in the shader object specified by shader.

 The compilation status will be stored as part of the shader object's state. This value will be set to GL_TRUE if
 the shader was compiled without errors and is ready for use, and GL_FALSE otherwise. It can be queried by calling glGetShader 
 with arguments shader and GL_COMPILE_STATUS.
 Compilation of a shader can fail for a number of reasons as specified by the OpenGL Shading Language Specification.
 Whether or not the compilation was successful, information about the compilation can be obtained from the shader object's
 information log by calling glGetShaderInfoLog.


   
   
   
   */
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID);

     if(std::string error = checkForShaderCompilationErrors(shaderID); error.size() != 0){
        std::cerr << "ERROR IN " << filename << std::endl;
        std::cerr << error << std::endl;
        glDeleteShader(shaderID); // there is an error in shader so delete it and return false 
        return false;
    }
 
    // attach the shader to the program then delete the shader
    // no error so attach shader to profram and then delete shader and return true

    /*
   Name
    glAttachShader — Attaches a shader object to a program object

   C Specification
     void glAttachShader(	GLuint program,
 	       GLuint shader);
 
   Parameters
    1-program : Specifies the program object to which a shader object will be attached.

    2-shader : Specifies the shader object that is to be attached.
    
    
    */
    glAttachShader(program, shaderID);
    glDeleteShader(shaderID);
  

  //We return true if the compilation succeeded
    return true;
    

    //TODO: Complete this function
    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.

}



bool our::ShaderProgram::link() const {
    /*
    Name
     glLinkProgram — Links a program object

    C Specification
      void glLinkProgram(	GLuint program);
 
    Parameters
      1-program : Specifies the handle of the program object to be linked.
    
    
    */
    glLinkProgram(program);

    
    // checkk for linking error if there is an error return false 
    if(auto error = checkForLinkingErrors(program); error.size() != 0){
        std::cerr << "LINKING ERROR" << std::endl;
        std::cerr << error << std::endl;
        return false;
    }

    return true;
      //TODO: Complete this function
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}