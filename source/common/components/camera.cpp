#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt


        // The lookAt() function takes three parameters: the camera's position, the position
        //  that the camera is looking at, and the up vector that defines the orientation of the camera.
        
        // eye is camera position, the last vector represents a position not a direction.
        // center position camera is looking at, the last vector represents a position not a direction.
        // upDirection is turn up vector, the last vector represents a direction and not a position, so it's 0
        glm::vec4 eye = glm::vec4(0, 0, 0, 1);
        glm::vec4 center = glm::vec4(0, 0, -1, 1);
        glm::vec4 upDirection = glm::vec4(0, 1, 0, 0);
         return glm::lookAt(
             glm::vec3(M * eye),
             glm::vec3(M * center),
             glm::vec3(M * upDirection)
        );
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        
        //aspect ratio (width ÷ height of the window)
        float aspectRatio = float(viewportSize.x) / float(viewportSize.y);

        // Left is  (-orthoHeight / 2) * aspectRatio.
        float Left = (-orthoHeight / 2) * aspectRatio;
        // Left is  (orthoHeight / 2) * aspectRatio.
        float Right = (orthoHeight / 2) * aspectRatio;
        // Bottom is -orthoHeight/2.
        float Bottom = -orthoHeight / 2;
        // Top is orthoHeight/2.
        float Top = orthoHeight / 2;

        // Defining an Orthographic Projection
        if (cameraType == CameraType::ORTHOGRAPHIC){
            // The ortho() function typically takes four parameters: left, right, bottom, top.
            // The ortho() function uses these parameters to create a 4x4 projection matrix that 
            // can be used to transform the coordinates of the 3D objects into 2D coordinates on the screen.
            glm::mat4 orthographic = glm::ortho(
            Left,
            Right,
            Bottom,
            Top
            );
            return orthographic;

        }
        // Defining a Perspective Projection
        else
        {
            // The perspective() function typically takes four parameters: fovy, aspect, near, far.
            // The perspective() function uses these parameters to create a 4x4 projection matrix that
            // can be used to transform the coordinates of the 3D objects into 2D coordinates on the screen, with the appropriate perspective distortion.
            glm::mat4 projection = glm::perspective(
                // The vertical field of view angle (smaller means more zoom in)
                fovY,
                //aspect ratio (width ÷ height of the window)
                aspectRatio,
                //near plane relative to camera
                near,
                // far plane relative to camera
                far);
            return  projection;
        }
    }
}