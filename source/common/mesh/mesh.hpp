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
            //     GLint size,
            //     GLenum type,
            //     GLboolean normalized,
            //     GLsizei stride,
            //     const void * pointer);

            // position_loc  
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
            
            // color_loc 
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4,  GL_UNSIGNED_BYTE, true, sizeof(Vertex),  (void*)offsetof(Vertex, color));
        
            //tex coord
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT , false, sizeof(Vertex),  (void*)offsetof(Vertex, tex_coord));    
        
            //normal
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
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
            //    GLenum mode,
            //   GLsizei count, <= Specifies the number of elements to be rendered with previously seted "elementCount"
            //   GLenum type,  <= we use "GL_UNSIGNED_INT" as VAO is (unsigned int)
            //   const void * indices <= we start from index 0 to take all the elements 
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