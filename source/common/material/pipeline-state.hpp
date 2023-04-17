#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 4) Write this function
        // documentation : norhan  
        /*
OpenGL is a state machine where the options we pick are stored in the OpenGL context and affect the
upcoming draw calls. Since each object may require different options while drawing (e.g. transparent objectsrequire blending while Opaque objects don't), we would need to store the options for each object in a data
structure and set the OpenGL options to match the given options before drawing.
This is where we use the "PipelineState" structure which we will use to store the depth testing, face culling,
blending and color/depth mask options. The setup function of the PipelineState sets the OpenGL options to
match the ones stored in the corresponding PipelineState instance.
        */

       /* */
            if (this->faceCulling.enabled) {
                glEnable(GL_CULL_FACE); //enable face culling
                glCullFace(this->faceCulling.culledFace);//remove the back face which is cw
                glFrontFace(this->faceCulling.frontFace);//define  the front face is  GL_CCW : counter clock wise 132 
                /*
                counter clock wise is front 
                      /3\
                     /   \    
                    /     \
                   1-------2

                    rotate around x and y  change the front and back faces
                    rotate around Z things keeps as it % 
                */

            } else {
                glDisable(GL_CULL_FACE); //disable face culling
            }



            if (this->depthTesting.enabled) {
                 //depth buffer: store a depth value for each pixel (sample in case MSAA)
                /* somtimes a problem of z fiting occurs so to solve it is to increase the precision of depth testing or depth 
                buffer by increasing the number of bits 
                or decrease the distance between near and far 
                so ir will have higher precision */ 
                glEnable(GL_DEPTH_TEST);//enable depth testing
                glDepthFunc(this->depthTesting.function); //define the function that is GL_LEQUAL which means that take the nearst value

              
            } else {
                glDisable(GL_DEPTH_TEST);//disable test depth
            }



            if (this->blending.enabled) {
                /*
                blending perform addition between source(thing that yoy are drawing) and destination(screen)

                (source.aplpha) * source  + (1-source.aplpha) * destination 

                   C = ∝s *Cs   +   (1 - ∝s)* CD

                    Sort from Farthest to Nearest.
                     Draw Opaque Objects first 
                */
                glEnable(GL_BLEND); //enable blending
                glBlendEquation(this->blending.equation); // GL_FUNC_ADD 
                glBlendFunc(this->blending.sourceFactor, this->blending.destinationFactor);//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBlendColor(this->blending.constantColor.r, this->blending.constantColor.g, this->blending.constantColor.b, this->blending.constantColor.a);//The GL_BLEND_COLOR may be used to calculate the source and destination blending factors. The color components are clamped to the range [0,1]
 before being stored.
            } else {
                glDisable(GL_BLEND); //disable blending
            }

            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);//specify whether the individual color components in the frame buffer can or cannot be written.
            glDepthMask(depthMask);//glDepthMask specifies whether the depth buffer is enabled for writing.


        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}