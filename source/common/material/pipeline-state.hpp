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
        // documentation   
        /*
OpenGL is a state machine where the options we pick are stored in the OpenGL context and affect the
upcoming draw calls. Since each object may require different options while drawing (e.g. transparent objectsrequire blending while Opaque objects don't), we would need to store the options for each object in a data
structure and set the OpenGL options to match the given options before drawing.
This is where we use the "PipelineState" structure which we will use to store the depth testing, face culling,
blending and color/depth mask options. The setup function of the PipelineState sets the OpenGL options to
match the ones stored in the corresponding PipelineState instance.
        */

       /* 
       Name
        glEnable — enable or disable server-side GL capabilities

       C Specification
        void glEnable(	GLenum cap);
        
       Parameters
        cap : Specifies a symbolic constant indicating a GL capability.

        C Specification
         void glDisable(	GLenum cap);
        
        Parameters
         cap : Specifies a symbolic constant indicating a GL capability.

         ////////////////////////////////////////////////////////////////////////////
         Name
            glCullFace — specify whether front- or back-facing facets can be culled

         C Specification
            void glCullFace(	GLenum mode);
            
        Parameters
            mode : Specifies whether front- or back-facing facets are candidates for culling. Symbolic constants GL_FRONT, GL_BACK, 
              and GL_FRONT_AND_BACK are accepted. The initial value is GL_BACK.
        
    ////////////////////////////////////////////////////////////////////////////////////////////////
     Name
        glFrontFace — define front- and back-facing polygons

    C Specification
        void glFrontFace(	GLenum mode);
    
    Parameters
        mode : Specifies the orientation of front-facing polygons. GL_CW and GL_CCW are accepted. The initial value is GL_CCW.

       //////////////////////////////////////////////////////////////////////////////////////////// 
       
       */
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

           /*
            Name
            glDepthFunc — specify the value used for depth buffer comparisons

            C Specification
            void glDepthFunc(	GLenum func);
            
            Parameters
            1-func : Specifies the depth comparison function. Symbolic constants GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS are accepted. The initial value is GL_LESS.

            Description
                glDepthFunc specifies the function used to compare each incoming pixel depth value with the depth value present in the depth buffer. The comparison is performed only if depth testing is enabled. (See glEnable and glDisable of GL_DEPTH_TEST.)

                func specifies the conditions under which the pixel will be drawn. The comparison functions are as follows:

                GL_NEVER
                Never passes.

                GL_LESS
                Passes if the incoming depth value is less than the stored depth value.

                GL_EQUAL
                Passes if the incoming depth value is equal to the stored depth value.

                GL_LEQUAL
                Passes if the incoming depth value is less than or equal to the stored depth value.

                GL_GREATER
                Passes if the incoming depth value is greater than the stored depth value.

                GL_NOTEQUAL
                Passes if the incoming depth value is not equal to the stored depth value.

                GL_GEQUAL
                Passes if the incoming depth value is greater than or equal to the stored depth value.

                GL_ALWAYS
                Always passes.

            The initial value of func is GL_LESS. Initially, depth testing is disabled. If depth testing is disabled or if no depth buffer exists, it is as if the depth test always passes.
           */

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

            /*
            Name
            glBlendEquation — specify the equation used for both the RGB blend equation and the Alpha blend equation

            C Specification
            void glBlendEquation(	GLenum mode);
            
            void glBlendEquationi(	GLuint buf,
                GLenum mode);
            
            Parameters
            1-buf : for glBlendEquationi, specifies the index of the draw buffer for which to set the blend equation.

            2-mode : specifies how source and destination colors are combined. It must be GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX.
                        
       ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            Name
            glBlendFunc — specify pixel arithmetic

            C Specification
            void glBlendFunc(	GLenum sfactor,
                GLenum dfactor);
            
            void glBlendFunci(	GLuint buf,
                GLenum sfactor,
                GLenum dfactor);
            
            Parameters
            1-buf
            For glBlendFunci, specifies the index of the draw buffer for which to set the blend function.

            2-sfactor
            Specifies how the red, green, blue, and alpha source blending factors are computed. The initial value is GL_ONE.

            3-dfactor
            Specifies how the red, green, blue, and alpha destination blending factors are computed. The following symbolic constants are accepted: GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA. GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, and GL_ONE_MINUS_CONSTANT_ALPHA. The initial value is GL_ZERO.
            

   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Name
            glBlendColor — set the blend color

            C Specification
            void glBlendColor(	GLfloat red,
                GLfloat green,
                GLfloat blue,
                GLfloat alpha);
            
            Parameters
            red, green, blue, alpha
            specify the components of GL_BLEND_COLOR

            Description
            The GL_BLEND_COLOR may be used to calculate the source and destination blending factors. The color components are clamped to the range [0,1]
            before being stored. See glBlendFunc for a complete description of the blending operations. Initially the GL_BLEND_COLOR is set to (0, 0, 0, 0). 
            */

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
                glBlendColor(this->blending.constantColor.r, this->blending.constantColor.g, this->blending.constantColor.b, this->blending.constantColor.a);//The GL_BLEND_COLOR may be used to calculate the source and destination blending factors. The color components are clamped to the range [0,1]before being stored.
            } else {
                glDisable(GL_BLEND); //disable blending
            }
            /*
            Name
            glColorMask, glColorMaski — enable and disable writing of frame buffer color components

            C Specification
            void glColorMask(	GLboolean red,
                GLboolean green,
                GLboolean blue,
                GLboolean alpha);
            
            void glColorMaski(	GLuint buf,
                GLboolean red,
                GLboolean green,
                GLboolean blue,
                GLboolean alpha);
            
            Parameters
            buf
            For glColorMaski, specifies the index of the draw buffer whose color mask to set.

            red, green, blue, alpha
            Specify whether red, green, blue, and alpha are to be written into the frame buffer. The initial values are all GL_TRUE, indicating that the color components are written.
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Name
        glDepthMask — enable or disable writing into the depth buffer

        C Specification
        void glDepthMask(	GLboolean flag);
        
        Parameters
        flag
        Specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

        Description
        glDepthMask specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.
            */

            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);//specify whether the individual color components in the frame buffer can or cannot be written.
            glDepthMask(depthMask);//glDepthMask specifies whether the depth buffer is enabled for writing.


        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}