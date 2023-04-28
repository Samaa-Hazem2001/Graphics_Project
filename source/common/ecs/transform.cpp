#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function

        //here we use the concept of "Translation Rotation Scaling (TRS)"
        //as Scaling,Rotation,Translation can also be stored in a 3D vector 
        //then we use them with scale,yawPitchRoll,translate by sending them with unit matrix 
        //so we will be apple to avoid the human errors in assigning matrices values

        glm::mat4 matrixScale = glm::scale(glm::mat4(1.0f), scale);
        //for more about yawPitchRoll animation to specify wi=hich one to which axis see this:
        // https://howthingsfly.si.edu/flight-dynamics/roll-pitch-and-yaw
        glm::mat4 matrixRotation = glm::yawPitchRoll(rotation.y,rotation.x,rotation.z);
        glm::mat4 matrixTranslation = glm::translate(glm::mat4(1.0f), position);

        //with order of TRS 
        return matrixTranslation * matrixRotation * matrixScale ;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}