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
            
            // GLuint vertex_array;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);


            // GLuint vertex_buffer;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(Vertex) , vertices.data(), GL_STATIC_DRAW);
        
            // GLint position_loc = 0; // glGetAttribLocation(program, "position");
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
            // glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position)));

            // GLint color_loc = 1; // glGetAttribLocation(program, "color");
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4,  GL_UNSIGNED_BYTE, true, sizeof(Vertex),  (void*)offsetof(Vertex, color));
        
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT , false, sizeof(Vertex),  (void*)offsetof(Vertex, tex_coord));    //norm
        
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3,  GL_FLOAT, false, sizeof(Vertex),  (void*)offsetof(Vertex, normal));
        


            // GLuint element_buffer;
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof( unsigned int), elements.data(), GL_STATIC_DRAW);  
        
            
            elementCount=(GLsizei)elements.size();
            // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

            // glBindVertexArray(0);

            // glBindVertexArray(vertex_array);
        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            // while (!glfwWindowShouldClose(window)) {
            // float time = (float)glfwGetTime();
            
            // glClearColor(0.0, 0.0, 0.0, 1.0);
            // glClear(GL_COLOR_BUFFER_BIT);

            // glUseProgram(program);
            glBindVertexArray(VAO);

            // glUniform1f(time_loc, time);
            // glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);

            // glUniform1f(time_loc, time + 1.5);
            // glDrawArrays(GL_TRIANGLES, 0, 6);

            // glfwSwapBuffers(window);
            // glfwPollEvents();
            // }
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}