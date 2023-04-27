#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            /*
            glCreateProgram : creates an empty program object and returns a non-zero value by which it can be referenced.
             A program object is an object to which shader objects can be attached. This provides a mechanism to specify the shader objects that will
              be linked to create a program. It also provides a means for checking the compatibility of the shaders that will be used to create a
               program (for instance, checking the compatibility
             between a vertex shader and a fragment shader). When no longer needed as part of a program object, shader objects can be detached.*/
            program=glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program

            /*
            Program objects can be deleted by calling glDeleteProgram().
             The memory associated with the program object will be deleted when it is no longer part of current rendering state for any context.
            */
            if(program) // check if there is a shader program then delete it 
            glDeleteProgram(program); 

        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }
        /*
        glGetUniformLocation — Returns the location of a uniform variable

        
         GLint glGetUniformLocation(	GLuint program , const GLchar *name);
         
         Parameters
           1-program : Specifies the program object to be queried.
           2-name : Points to a null terminated string containing the name of the uniform variable whose location is to be queried.

         Description
         glGetUniformLocation returns an integer that represents the location of a specific uniform variable within a program object. 
         name must be a null terminated string that contains no white space. name must be an active uniform variable name in program that 
         is not a structure, an array of structures, or a subcomponent of a vector or a matrix. This function returns -1 if
         name does not correspond to an active uniform variable in program, if name starts with the reserved prefix "gl_",
         or if name is associated with an atomic counter or a named uniform block.

         Uniform variables that are structures or arrays of structures may be queried by calling glGetUniformLocation for each field within 
         the structure. The array element operator "[]" and the structure field operator "." may be used in name in order to select elements 
         within an array or fields within a structure. The result of using these operators is not allowed to be another structure, an array of 
         structures, or a subcomponent of a vector or a matrix. Except if the last part of name indicates a uniform variable array, 
         the location of the first element of an array can be retrieved by using the name of the array, or by using the name appended by "[0]".

         The actual locations assigned to uniform variables are not known until the program object is linked successfully. 
         After linking has occurred, the command glGetUniformLocation can be used to obtain the location of a uniform variable. 
         This location value can then be passed to glUniform to set the value of the uniform variable or to glGetUniform in order 
         to query the current value of the uniform variable. After a program object has been linked successfully, the index values
         for uniform variables remain fixed until the next link command occurs. Uniform variable locations and values can only be
         queried after a link if the link was successful.


         ////////////////////////////////////////////////////////////////////////////////////

         glUniform — Specify the value of a uniform variable for the current program object

         C Specification

            void glUniform1f(	GLint location,
                GLfloat v0);
            
            void glUniform2f(	GLint location,
                GLfloat v0,
                GLfloat v1);
            
            void glUniform3f(	GLint location,
                GLfloat v0,
                GLfloat v1,
                GLfloat v2);
            
            void glUniform4f(	GLint location,
                GLfloat v0,
                GLfloat v1,
                GLfloat v2,
                GLfloat v3);
            
            void glUniform1i(	GLint location,
                GLint v0);
            
            void glUniform2i(	GLint location,
                GLint v0,
                GLint v1);
            
            void glUniform3i(	GLint location,
                GLint v0,
                GLint v1,
                GLint v2);
            
            void glUniform4i(	GLint location,
                GLint v0,
                GLint v1,
                GLint v2,
                GLint v3);
            
            void glUniform1ui(	GLint location,
                GLuint v0);
            
            void glUniform2ui(	GLint location,
                GLuint v0,
                GLuint v1);
            
            void glUniform3ui(	GLint location,
                GLuint v0,
                GLuint v1,
                GLuint v2);
            
            void glUniform4ui(	GLint location,
                GLuint v0,
                GLuint v1,
                GLuint v2,
                GLuint v3);
            
            void glUniform1fv(	GLint location,
                GLsizei count,
                const GLfloat *value);
            
            void glUniform2fv(	GLint location,
                GLsizei count,
                const GLfloat *value);
            
            void glUniform3fv(	GLint location,
                GLsizei count,
                const GLfloat *value);
            
            void glUniform4fv(	GLint location,
                GLsizei count,
                const GLfloat *value);
            
            void glUniform1iv(	GLint location,
                GLsizei count,
                const GLint *value);
            
            void glUniform2iv(	GLint location,
                GLsizei count,
                const GLint *value);
            
            void glUniform3iv(	GLint location,
                GLsizei count,
                const GLint *value);
            
            void glUniform4iv(	GLint location,
                GLsizei count,
                const GLint *value);
            
            void glUniform1uiv(	GLint location,
                GLsizei count,
                const GLuint *value);
            
            void glUniform2uiv(	GLint location,
                GLsizei count,
                const GLuint *value);
            
            void glUniform3uiv(	GLint location,
                GLsizei count,
                const GLuint *value);
            
            void glUniform4uiv(	GLint location,
                GLsizei count,
                const GLuint *value);
            
            void glUniformMatrix2fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix3fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix4fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix2x3fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix3x2fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix2x4fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix4x2fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix3x4fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            
            void glUniformMatrix4x3fv(	GLint location,
                GLsizei count,
                GLboolean transpose,
                const GLfloat *value);
            

            Parameters

                1-location : Specifies the location of the uniform variable to be modified.

                2-count :  For the vector (glUniform*v) commands, specifies the number of elements that are to be modified. This should be 1 if 
                           the targeted uniform variable is not an array, and 1 or more if it is an array.

                            For the matrix (glUniformMatrix*) commands, specifies the number of matrices that are to be modified.
                            This should be 1 if the targeted uniform variable is not an array of matrices, and 1 or more if it is an array
                            of matrices.

                3-transpose : For the matrix commands, specifies whether to transpose the matrix as the values are loaded into the uniform 
                variable.

                v0, v1, v2, v3
                For the scalar commands, specifies the new values to be used for the specified uniform variable.

                4-value : For the vector and matrix commands, specifies a pointer to an array of count values that will be used to update 
                the specified uniform variable.
        */
        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program,name.c_str());

        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform),value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform),value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform),value);

        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
             glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }
 
        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
             glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, false, glm::value_ptr(matrix));
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        //Question: Why do we delete the copy constructor and assignment operator?
        ShaderProgram(ShaderProgram const &) = delete; // Delete the copy constructor
        ShaderProgram &operator=(ShaderProgram const &) = delete; //Delete the assignment operator

        /*
        The reason to delete the copy constructor and assignment operator is to prevent unintended behavior that may occur when an object 
        is copied or assigned.

        When an object is copied or assigned, the copy constructor and assignment operator are called to create a new object that is a 
        duplicate of the original. However, in some cases, copying an object may not be appropriate or may lead to unintended consequences.
        For example, if an object contains a resource that should not be duplicated, such as a file handle or a network connection, 
        copying the object could result in multiple objects trying to access the same resource, leading to errors or unexpected behavior.
        By deleting the copy constructor and assignment operator, you can ensure that the object can only be moved, 
        preventing any unintended copying or assignment. This can be particularly important for objects that manage resources,
        as it helps to prevent memory leaks and other issues that could arise from improper handling of the resources.

        Note that in C++11 and later versions, you can use the = delete syntax to explicitly delete the copy constructor 
        and assignment operator, making it clear to other developers that copying or assignment is not allowed.
        
        */

      



    };

}

#endif