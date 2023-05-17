#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 6) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 6) Write this function
        Material::setup();
        shader->set("tint",tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 6) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold",alphaThreshold);
        if(texture != NULL && sampler !=NULL)
        {
        glActiveTexture(GL_TEXTURE0); //we send it unit 0 
        texture->bind();
        sampler->bind(0);
        shader->set("tex",0);
        }
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){

        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    // ------------------- light material ------------------- //
     void LightMaterial::setup() const {
        // call setup function
        TexturedMaterial::setup(); 
        // if it's albedo
        if (albedo){
            // Here we set the active texture unit to 0 
            glActiveTexture(GL_TEXTURE0);
            // then bind the texture to it
            albedo->bind();
            // binds this sampler to 0
            sampler->bind(0);
            // set albedo to 0
            shader->set("material.albedo",0);
        }

        // if it's specular
        if (specular){
            // Here we set the active texture unit to 1
            glActiveTexture(GL_TEXTURE1);  
            // then bind the texture to it
            specular->bind();
            // binds this sampler to 1
            sampler->bind(1);
            // set specular to 1
            shader->set("material.specular",1);
        }
        
        // if it's ambient_occlusion
        if (ambient_occlusion){
            // Here we set the active texture unit to 2
            glActiveTexture(GL_TEXTURE2);  
            // then bind the texture to it
            ambient_occlusion->bind();
            // binds this sampler to 2
            sampler->bind(2);
            // set ambient_occlusion to 1
            shader->set("material.ambient_occlusion",2);
        }
        
        // if it's roughness
        if (roughness){
            // Here we set the active texture unit to 3
            glActiveTexture(GL_TEXTURE3);  
            // then bind the texture to it
            roughness->bind();
            // binds this sampler to 3
            sampler->bind(3);
            // set roughness to 3
            shader->set("material.roughness",3);
        }
  
        // if it's emissive
        if (emissive){
            // Here we set the active texture unit to 4
            glActiveTexture(GL_TEXTURE4); 
            // then bind the texture to it 
            emissive->bind();
            // binds this sampler to 4
            sampler->bind(4);
            // set emissive to 4
            shader->set("material.emissive",4);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    // This function read the material data from a json object
    void LightMaterial::deserialize(const nlohmann::json& data){
        
        // read the texture data from a json object
        TexturedMaterial::deserialize(data); 

        if(!data.is_object()) return;

        // if data contains albedo
        if(data.contains("albedo")){
            // get the value of it
           albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        }

        // if data contains specular
        if(data.contains("specular")){
            // get the value of it
           specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        }

        // if data contains ambient_occlusion
        if(data.contains("ambient_occlusion")){
            // get the value of it
           ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        }
        
        // if data contains roughness
        if(data.contains("roughness")){
            // get the value of it
           roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        }

        // if data contains emissive
        if(data.contains("emissive")){
            // get the value of it
           emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        }
        
        // get the value of sampler
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    }

}