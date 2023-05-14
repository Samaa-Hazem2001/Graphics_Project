#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/running-object.hpp"
#include "../components/collision.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "iostream"

#include<windows.h>
#include <Mmsystem.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem {
        Application* app; // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent* camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            RunningObject *running = nullptr;

            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with RunningObject, we can do nothing so we return
            for (auto entity : world->getEntities())
            {
                running = entity->getComponent<RunningObject>();
                if (running)
                    break;
            }

            // If there is no entity with running, we can do nothing so we return
            if(!(running)) return;
            // Get the entity that we found via getOwner of RunningObject (we could use controller->getOwner())

            Entity* entity = camera->getOwner();
            // Entity* entity = running->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked){
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
            // If the left mouse button is released, we unlock and unhide the mouse.
            } else if(!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position
            glm::vec3 world_position = entity->getLocalToWorldMatrix()*glm::vec4(0,0,0,1);
            // glm::vec3 position = lightSources[i]->getOwner()->getLocalToWorldMatrix()*glm::vec4(0,0,0,1);
            glm::vec3& position = entity->localTransform.position;

            // create instatse of CollisionComponent
            Entity *collisionEntity = nullptr;
            CollisionComponent *Collision = nullptr;



            // for loop for all intities in the world:
            for (auto entity : world->getEntities())
            {
                // try to found Collision entity
                Collision = entity->getComponent<CollisionComponent>();
                // if not found, continue
                if (!(Collision)) continue;
                // if found, get access of it.
                collisionEntity = Collision->getOwner();

                // access position for the collisionEntity
                glm::vec3 &collisionPosition = collisionEntity->localTransform.position;

                // collisionX that refer to if there is a collision in x axis
                bool collisionX = false;
                 // collisionZ that refer to if there is a collision in y axis
                bool collisionZ = false;
                // if collision type is penalty:
                if (Collision->getCollisionType() == "penalty")
                {
                    // check if it's hit it or not and store it in collisionX
                    collisionX = world_position.x + 0.2 >= collisionPosition.x &&
                                 collisionPosition.x + 0.2 >= world_position.x;

                    collisionZ = world_position.z + 2 >= collisionPosition.z &&
                                  collisionPosition.z + 2 >= world_position.z;
                }
                // if collision type is reward:
                else
                {
                    // check if it's hit it or not and store it in collisionX
                    collisionX = world_position.x + 1.5 >= collisionPosition.x &&
                                 collisionPosition.x + 1.5 >= world_position.x;

                    collisionZ = world_position.z + 6 >= collisionPosition.z &&
                                  collisionPosition.z + 6 >= world_position.z;
                }
               

                // if there is collision in both x, y axies
                if (collisionX && collisionZ)
                {
                    // reomve the collision entity
                    world->markForRemoval(collisionEntity);
                    // if collision type is penalty:
                    if (Collision->getCollisionType() == "penalty"){
                        // if(app->reward == 0 || app->reward < 0){
                        //     app->penalty = true;
                        //     app->reward = 0;
                        //     return;
                        // }
                        // app->reward -= 10;
                        // if(app->reward == 0 || app->reward < 0){
                        //     app->penalty = true;
                        //     app->reward = 0;
                        //     return;
                        // }
                        // PlaySound("assets/sound/lose.wav", NULL, SND_ASYNC);

                        // make penalty to true
                        app->penalty = true;
                    }
                    // if collision type is reward:
                    else{
                        // play win sound
                        PlaySound("assets/sound/win.wav", NULL, SND_ASYNC);
                        // add 10 for rewards
                        app->reward += 10;

                    }

                    break;
                }
            }

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)), 
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            
            // make it move forward all the time
            position += front * (deltaTime * current_sensitivity.z);
            
            if (position.x < 7)
            {
                // D or left button moves the player left
                if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * (deltaTime * current_sensitivity.x);
                if (app->getKeyboard().isPressed(GLFW_KEY_RIGHT)) position += right * (deltaTime * current_sensitivity.x);
            }
            if (position.x > -7)
            {
                // A or right button moves the player right
                if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * (deltaTime * current_sensitivity.x);
                if (app->getKeyboard().isPressed(GLFW_KEY_LEFT)) position -= right * (deltaTime * current_sensitivity.x);
            }
           }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

    };

}
