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

        lightType = data.value("lightType", "DIRECTIONAL");

        if (lightType == "DIRECTIONAL")
            type = 0;

        else if (lightType == "POINT")
            type = 1;

        else if (lightType == "SPOT")
            type =2;

       
        diffuse = glm::vec3(data.value("diffuse", glm::vec3(1, 1, 1)));
        specular = glm::vec3(data.value("specular", glm::vec3(1, 1, 1)));
        if (type != 0)
        {
        attenuation = glm::vec3(data.value("attenuation", glm::vec3(1, 0, 0)));
        }

        if (type ==2)
        {  
        coneAngles.x = glm::radians((float)data.value("coneAngles.in",10));
        coneAngles.y = glm::radians((float)data.value("coneAngles.out",80));
        }
    }
    
}