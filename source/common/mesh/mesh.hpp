#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            
            //for vertex_array & vertex_buffer & element_buffer below , we specified the number of the array/buffer object -first parameter- to be generated to be only 1 -for each array/buffer-
            //for the second parameter we send the address to the array/buffer object (defined above)
            //then we bind each array/buffer 

            // vertex_array 
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);


            // vertex_buffer:

            // void glBufferData(	
            //    GLenum target, <= here we specify the target to which the buffer object is bound for , whish is here "GL_ARRAY_BUFFER"
            //     GLsizeiptr size,  <= we multiply with "sizeof( Vertex)" as vertices defined as "std::vector<Vertex>"
            //     const void * data, <= the data of the vertices
            //     GLenum usage <= we use "GL_STATIC_DRAW" as we are not going to change contents of buffer often
            // );
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(Vertex) , vertices.data(), GL_STATIC_DRAW);
        
            // void glVertexAttribPointer(	
            //     GLuint index,
            //     GLint size, <= The number of components per attribute
            //     GLenum type, <=The data type of each component of the color vector
            //     GLboolean normalized, <=Whether the data should be normalized before being passed to the shader program or not
            //     GLsizei stride, <=Specifies the byte offset between consecutive generic vertex attributes(This tells OpenGL how much memory to skip to get to the next vertex's normal data)
            //     const void * pointer <= The offset in bytes from the start of each vertex to the specific attribute
            // );

            // position_loc  
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            //ATTRIB_LOC_POSITION: is the location of the position attribute in the shader program
            //3: the position is represented as a 3-component vector (xyz)
            //false: means that the data should not be normalized before it is used#pragma endregion
            //sizeof(Vertex) specifies the size of each vertex in bytes
            //last parameter : specifies the offset from the beginning of each vertex to the start of its position data (which is 0 since it's at the beginning)
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
            

            // color_loc :
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            //ATTRIB_LOC_TEXCOORD: is the location of the attribute in the shader program.
            //4: the color is represented as a 4-component vector (RGBA)
            //GL_UNSIGNED_BYTE: color components are stored as unsigned bytes.
            //true: In this case, the color components are normalized to the range [0, 1] before being passed.
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4,  GL_UNSIGNED_BYTE, true, sizeof(Vertex),  (void*)offsetof(Vertex, color));
        

            //tex coord
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            //2: specifies that there are two components (u and v) in each texture coordinate
            //GL_FLOAT :each component is a floating-point value.
            //last parameter specifies the offset from the beginning of a vertex to its texture coordinate data
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT , false, sizeof(Vertex),  (void*)offsetof(Vertex, tex_coord));    
        

            //normal
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            //ATTRIB_LOC_NORMAL: is an integer constant that represents the location index of the normal attribute in the shader program
            //3 : specifies that each normal vector has three components (x, y, z)
            //GL_FLOAT: indicates that each component is a floating-point value. 
            //false: means that the data should not be normalized before it is used#pragma endregion
            //last parameter: specifies the offset from the beginning of a Vertex struct to its normal member variable. This tells OpenGL where to find the first normal vector in memory for a given vertex.
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3,  GL_FLOAT, false, sizeof(Vertex),  (void*)offsetof(Vertex, normal));
        
            // element_buffer:

            // void glBufferData(	
            //    GLenum target, <= here we specify the target to which the buffer object is bound for , whish is here "GL_ELEMENT_ARRAY_BUFFER"
            //     GLsizeiptr size,  <= we multiply with "sizeof( unsigned int)" as elements defined as "std::vector<unsigned int>"
            //     const void * data, <= the data of the elements
            //     GLenum usage <= we use "GL_STATIC_DRAW" as we are not going to change contents of buffer often
            // );
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof( unsigned int), elements.data(), GL_STATIC_DRAW);  
        
            // set elementCount
            elementCount=(GLsizei)elements.size();
            
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function

            //void glDrawElements(	
            //    GLenum mode, <= we use here GL_TRIANGLES 
            //   GLsizei count, <= Specifies the number of elements to be rendered with previously seted "elementCount"
            //   GLenum type,  <= we use "GL_UNSIGNED_INT" as VAO is (unsigned int)
            //   const void * indices <=  Since we are starting at the beginning of the index buffer, we pass 0 as the offset
            // );

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function

            //for all buffers below , we specified the number of the array object -first parameter- to be deleted to be only 1 -for each buffer-
            //for the second parameter we send the address to the array object (defined above)
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}