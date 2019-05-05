//
//  Camera.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 02/05/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

class Camera
{
public:
    explicit Camera(vec3 position, vec3 front, vec3 up, float speed = 0.05f, float sensitivity = 0.05f);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    void forward();
    void backward();
    void left();
    void right();

    void calculateFrontVector(int32_t xPos, int32_t yPos);

    mat4 getViewMatrix() const;

private:
    vec3 _position;
    vec3 _front;
    vec3 _up;
    float _speed;
    float _sensitivity;

    static float _yaw;
    static float _pitch;
};

#endif /* Camera_hpp */
