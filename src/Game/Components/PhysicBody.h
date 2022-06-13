#pragma once

#include <memory>

#include <box2d/box2d.h>

struct PhysicBody {
    PhysicBody()
        : collide(false)
        , body(nullptr)
    {

    }
    bool collide;

    // Memory from box2d allocator
    // TODO: Check it
    b2Body* body;
};