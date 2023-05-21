#include "light.hpp"
#include "../deserialize-utils.hpp"
#include "./component-deserializer.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

namespace our
{
    // Deserializes the light data from a json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        // access the type of the light and set the default to be DIRECTIONAL.
        lightType = data.value("lightType", "DIRECTIONAL");

        // check if type of the light is DIRECTIONAL:
        if (lightType == "DIRECTIONAL")
            // set type = 0
            type = 0;

        // check if type of the light is POINT:
        else if (lightType == "POINT")
            // set type = 1
            type = 1;

        // check if type of the light is SPOT:
        else if (lightType == "SPOT")
            // set type = 2
            type =2;

        // set diffuse value
        diffuse = glm::vec3(data.value("diffuse", glm::vec3(1, 1, 1)));
        // set specular value
        specular = glm::vec3(data.value("specular", glm::vec3(1, 1, 1)));

        // check if type != 0, it's not DIRECTIONAL light, because DIRECTIONAL light doesn't have attenuation
        if (type != 0)
            // set attenuation value 
            attenuation = glm::vec3(data.value("attenuation", glm::vec3(1, 0, 0)));

        // check if type = 2, it's SPOT light
        if (type ==2) {  
            // set cone angles (inner, outer)
            coneAngles.x = glm::radians((float)data.value("coneAngles.in",10));
            coneAngles.y = glm::radians((float)data.value("coneAngles.out",80));
        }
    }
    
}