#pragma once
#include <glm/glm.hpp>

#include "View.h"

class OrthographicCamera : public View
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);
    void SetProjection(float left, float right, float bottom, float top);
    
protected:
    void RecalculateViewMatrix() override;
    
};
