#pragma once
#include "glm.hpp"
#include "transform.hpp"
#include <string>

struct Bounds {
public:
    double x, y, width, height;
    Bounds(): x(.0f), y(.0f), width(1.f), height(1.f) {};
    Bounds(double x, double y, double width, double height): x(x), y(y), width(width), height(height) {};
    glm::mat4 getTransform() {
        return glm::translate(glm::vec3(this->x, this->y, .0f))
               * glm::scale(glm::vec3(this->width * .5f, this->height * .5f, .0f));
    }
    Bounds getRelativePosition(const Bounds& parent) const {
        double left = (this->x - parent.x) / parent.width;
        double bottom = (this->y - parent.y) / parent.height;
        double widthDelta = this->width / parent.width;
        double heightDelta = this->height / parent.height;
        double width1 = widthDelta * 2.0;
        double height1 = heightDelta * 2.0;
        double left1 = left * 2.0 - 1.0 + width1 / 2.0;
        double bottom1 = bottom * 2.0 - 1.0 + height1 / 2.0;
        return {left1, bottom1, width1, height1};
    }
};

class Utilities {
public:
};
