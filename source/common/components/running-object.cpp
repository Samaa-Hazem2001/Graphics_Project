#include "running-object.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void RunningObject::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
    }
}