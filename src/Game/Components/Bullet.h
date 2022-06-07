#pragma once

struct Bullet {
	Bullet()
        : _collided(false)
        , speed(3.0f)
    {
    
    }
	float speed;
    bool _collided; // TODO: To compose ?
};
