#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

namespace our {

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //TODO: (Req 10) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            skyPipelineState.depthTesting.enabled = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            //TODO: (Req 11) Create a framebuffer
            /*
            here we are performing the post process here we are drawing on the texture it self it is something like making mirror :)
            ///////////////////////////////////////////////////////////////////////
            Name
              glGenFramebuffers — generate framebuffer object names

           C Specification
            void glGenFramebuffers(	GLsizei n,
                GLuint *ids);
 
           Parameters
            1-n : Specifies the number of framebuffer object names to generate.

            2-ids : Specifies an array in which the generated framebuffer object names are stored.

            /////////////////////////////////////////////////////////////////////////////////////
            Name
              glBindFramebuffer — bind a framebuffer to a framebuffer target

            C Specification
                void glBindFramebuffer(	GLenum target,
                    GLuint framebuffer);
                
            Parameters
                1-target : Specifies the framebuffer target of the binding operation.

                2-framebuffer : Specifies the name of the framebuffer object to bind.
            */
           
              glGenFramebuffers(1, &postprocessFrameBuffer); // Create a framebuffer
              glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer); // bind a framebuffer(texture that we will draw on ,buffer)
            //TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits).
             colorTarget = texture_utils::empty(GL_RGBA8, windowSize);//color 

            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);//depth texture

            /*
Name
     glFramebufferTexture2D — attach a texture image to a framebuffer object

C Specification
    void glFramebufferTexture2D(	GLenum target,
        GLenum attachment,
        GLenum textarget,
        GLuint texture,
        GLint level);
 
Parameters
        1-target : Specifies the framebuffer target. The symbolic constant must be GL_FRAMEBUFFER.

        2-attachment : Specifies the attachment point to which an image from texture should be attached. Must be one of the following symbolic constants: GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, or GL_STENCIL_ATTACHMENT.

        3-textarget : Specifies the texture target. Must be one of the following symbolic constants: GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, or GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.

        4-texture : Specifies the texture object whose image is to be attached.

        5-level : Specifies the mipmap level of the texture image to be attached, which must be 0.
            
            */

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);//attach depth

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);//attach color
            
            //TODO: (Req 11) Unbind the framebuffer just to be safe
            // pass to it zero to unvind the buffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // unbind the frame buffer 
            // Create a vertex array to use for drawing the texture
            /*
            
         Name
             glGenVertexArrays — generate vertex array object names

         C Specification
            void glGenVertexArrays(	GLsizei n,
                GLuint *arrays);
 
         Parameters
            1-n : Specifies the number of vertex array object names to generate.

            2-arrays : Specifies an array in which the generated vertex array object names are stored.
            
            */
            glGenVertexArrays(1, &postProcessVertexArray);//postProcessVertexArray vertex array 

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler* postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram* postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world){
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        auto owner = camera->getOwner();
        auto M = owner->getLocalToWorldMatrix();
        glm::vec3 eye = M * glm::vec4(0, 0, 0, 1);
        glm::vec3 center = M * glm::vec4(0, 0, -1, 1);
        glm::vec3 cameraForward = glm::normalize(center - eye);
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
            //TODO: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 
            // dot returns the dot product of two vectors
            if (glm::dot(cameraForward,first.center) > glm::dot(cameraForward,second.center)  )
                return true;
            else
                return false;
        });

        //TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        glm::mat4 P = camera->getProjectionMatrix(windowSize);
        glm::mat4 V = camera->getViewMatrix();
        glm::mat4 VP =  P*V ;

        //TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        glViewport(0, 0, windowSize.x, windowSize.y);

        //TODO: (Req 9) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0);

        //TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        glColorMask(true, true, true, true);
        glDepthMask(true);

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //TODO: (Req 11) bind the framebuffer
              glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
            
        }

        //TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //TODO: (Req 9) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        
        for (auto command : opaqueCommands)
        {
            command.material->setup();
            command.material->shader->set("transform", VP * command.localToWorld);
            command.mesh->draw();
        }


        // If there is a sky material, draw the sky
        if(this->skyMaterial){
            //TODO: (Req 10) setup the sky material
            this->skyMaterial->setup();

            //TODO: (Req 10) Get the camera position
            glm::vec3 camPosition = eye;

            //TODO: (Req 10) Create a model matrix for the sy such that it always follows the camera (sky sphere center = camera position)
            our::Transform skyTransform;
            skyTransform.position = camPosition;
            glm::mat4 skyModel = skyTransform.toMat4();

            //TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f
            );
            //TODO: (Req 10) set the "transform" uniform
            skyMaterial->shader->set("transform", alwaysBehindTransform * VP * skyModel);

            //TODO: (Req 10) draw the sky sphere
            skySphere->draw();
        }
        //TODO: (Req 9) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (auto command : transparentCommands)
        {
            command.material->setup();
            command.material->shader->set("transform", VP * command.localToWorld);
            command.mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //TODO: (Req 11) Return to the default framebuffer
             glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            //TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            glBindVertexArray(this->postProcessVertexArray);
            /*
Name
  glDrawArrays — render primitives from array data

C Specification
    void glDrawArrays(	GLenum mode,
        GLint first,
        GLsizei count);
 
Parameters
        1-mode : Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.

        2-first : Specifies the starting index in the enabled arrays.

        3-count : Specifies the number of indices to be rendered.
            */
            glDrawArrays(GL_TRIANGLES, 0, 3);
            
        }
    }

}