#pragma once

#include "../ecs/component.hpp"
#include "../ecs/transform.hpp"
#include <unordered_map>
#include <string>
#include <type_traits>
#include <glm/glm.hpp>


namespace our {

// An enum that defines the type of the light source
// We will support 3 types of lights.
// 1- Directional Light: where we assume that the light rays are parallel. We use this to approximate sun light.
// 2- Point Light: where we assume that the light source is a single point that emits light in every direction. It can be used to approximate light bulbs.
// 3- Spot Light: where we assume that the light source is a single point that emits light in the direction of a cone. It can be used to approximate torches, highway light poles.
    enum class LightType {
        DIRECTIONAL,
        SPOT,
        POINT
    };
    class LightComponent : public Component {
    public:
        int type; // The type of the light
        // We also define the color & intensity of the light for each component of the Phong model (Ambient, Diffuse, Specular).
        glm::vec3 diffuse, specular;
        glm::vec4 color;
        glm::vec3 attenuation; // x*d^2 + y*d + z
        glm::vec2 coneAngles; // x: inner_angle, y: outer_angle
        std::string lightType;
        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
        // identify it is a light component
        static std::string getID() { return "light"; }

    };

}