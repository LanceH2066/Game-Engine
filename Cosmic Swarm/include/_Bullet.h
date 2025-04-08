#ifndef _BULLET_H
#define _BULLET_H

#include<_common.h>
#include<_textureLoader.h>
#include<_timer.h>

class _Bullet
{
    public:
    _Bullet();
    virtual ~_Bullet();
    _textureLoader *textureLoader = new _textureLoader();
    float xMin, xMax, yMin, yMax;
    _timer *timer = new _timer();
    int t = 0;
    enum {IDLE, SHOOT, HIT};
    int actionTrigger;
    bool isAlive = false;
    vec3 initialPosition, position, direction, rotation, scale;
    vec3 collisionBoxSize = {0.15f, 0.6f, 1.0f};  // Half-extents, tweak based on PNG size
    float damage = 5.0f;

    void init(vec3 spawnPos, vec3 playerRotation, vec3 targetPos, _textureLoader* loader);
    void reset(vec3);
    void update(float deltaTime);
    void actions();
    void drawBullet();
    vec3 getCollisionBoxMin() const { return {position.x - collisionBoxSize.x, position.y - collisionBoxSize.y, position.z - collisionBoxSize.z}; }
    vec3 getCollisionBoxMax() const { return {position.x + collisionBoxSize.x, position.y + collisionBoxSize.y, position.z + collisionBoxSize.z}; }

    vector<vec3> getRotatedCorners() const;
    vector<int> hitEnemies;
    protected:

    private:
};

#endif // _BULLET_H
