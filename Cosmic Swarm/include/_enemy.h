#ifndef _ENEMY_H
#define _ENEMY_H
#include <_common.h>
#include <_timer.h>
#include <_textureLoader.h>
#include <_particleSystem.h>
#include <_xporb.h>

class _enemy
{
    public:
        _enemy();
        virtual ~_enemy();

        _timer* enemyTimer;
        _textureLoader* enemyTextureLoader;
        _particleSystem* explosionEffect;

        vec3 position;
        vec3 rotation;
        vec2 scale;

        vec3 playerPosition;

        float xMax, xMin, yMax, yMin;
        float speed;
        int actionTrigger;
        bool isAlive;

        void initEnemy(char* fileName, float hp, vec3 hitboxSize, float mSpeed);
        void drawEnemy(GLuint, float);
        void placeEnemy(vec3);
        void enemyActions(float deltaTime);
        void takeDamage(float damage,vector<_xpOrb>& xpOrbs, _textureLoader* xpOrbTexture);

        float maxHp, currentHp;
        float stoppingDistance = 0.35f;
        vec3 collisionBoxSize;
        vec3 getCollisionBoxMin() const { return {position.x - collisionBoxSize.x, position.y - collisionBoxSize.y, position.z - collisionBoxSize.z}; }
        vec3 getCollisionBoxMax() const { return {position.x + collisionBoxSize.x, position.y + collisionBoxSize.y, position.z + collisionBoxSize.z}; }

        vector<vec3> getRotatedCorners() const;

        bool startFlash;
        float flashDuration = 0.2f;
        float flashTimer = 0.0f;
        bool hasExploded;

    protected:
    private:
};

#endif // _ENEMY_H
