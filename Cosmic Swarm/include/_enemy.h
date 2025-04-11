#ifndef _ENEMY_H
#define _ENEMY_H

#include<_common.h>
#include<_timer.h>
#include<_textureLoader.h>
#include<_player.h>
#include <_particleSystem.h>
class _enemy
{
    public:
        _enemy();
        virtual ~_enemy();

        // Timer / Texture Loader
        _timer * enemyTimer = new _timer();
        _textureLoader * enemyTextureLoader = new _textureLoader();
        _particleSystem* explosionEffect;

        // Position / Rotation / Scale
        vec3 position;
        vec3 rotation;
        vec2 scale;

        //For enemies to track player
        void setPlayerReference(_player* player);
        _player* targetPlayer = new _player();

        // Animation
        float xMax, xMin, yMax, yMin;
        float speed;
        int actionTrigger;
        bool isAlive;

        // Methods
        void initEnemy(char* fileName);
        void drawEnemy(GLuint,float);
        void placeEnemy(vec3);
        void enemyActions(float deltaTime);
        void takeDamage(float damage);

        float maxHp, currentHp;
        float stoppingDistance = 1.0f;
        // Add these for collision
        vec3 collisionBoxSize = {0.6f, 0.6f, 1.0f};  // Half-extents of the collision box
        vec3 getCollisionBoxMin()const { return {position.x - collisionBoxSize.x, position.y - collisionBoxSize.y, position.z - collisionBoxSize.z}; }
        vec3 getCollisionBoxMax()const { return {position.x + collisionBoxSize.x, position.y + collisionBoxSize.y, position.z + collisionBoxSize.z}; }

        vector<vec3> getRotatedCorners() const;

        bool startFlash;
        float flashDuration = 0.2f;  // Duration of flash in seconds
        float flashTimer = 0.0f;     // Tracks elapsed flash time
        bool hasExploded; // Track if explosion has been triggered
    protected:

    private:
};

#endif // _ENEMY_H
