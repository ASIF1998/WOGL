//
//  Camera.cpp
//  WOGL
//
//  Created by Асиф Мамедов on 02/05/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Camera.hpp"

#include <iostream>

using namespace std;

float Camera::_yaw = 0.0f;
float Camera::_pitch = 45.0f;

Camera::Camera(vec3 position, vec3 front, vec3 up, float speed, float sensitivity) :
    _position{position},
    _front{front},
    _up{up},
    _speed{speed},
    _sensitivity{sensitivity}
{
}

void Camera::forward()
{
    _position += _front * _speed;
}

void Camera::backward() 
{
    _position -= _front * _speed;
}

void Camera::left()
{
    _position += normalize(cross(_front, _up)) * _speed;
}

void Camera::right()
{
    _position -= normalize(cross(_front, _up)) * _speed;
}

mat4 Camera::getViewMatrix() const 
{
    return lookAt(_position, _position + _front, _up);
}

void Camera::calculateFrontVector(int32_t xPos, int32_t yPos)
{
    static int32_t _lastX = 0;
    static int32_t _lastY = 0;

    float xoffset = (static_cast<float>(xPos - _lastX) / numeric_limits<int32_t>::max()) * _sensitivity * 1000000000.0f;
    float yoffset = (static_cast<float>(_lastY - yPos) / numeric_limits<int32_t>::max()) * _sensitivity * 1000000000.0f;

    _lastX = xPos;
    _lastY = yPos;

    _yaw -= xoffset;
    _pitch -= yoffset;

    if(_pitch > 90.0f) {
        _pitch = 90.0f;
    } else if(_pitch < -90.0f) {
        _pitch = -90.0f;
    }

    _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front.y = sin(glm::radians(_pitch));
    _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _front = normalize(_front);
}
