#ifndef _ENEMY_H
#define _ENEMY_H

#include<_common.h>
#include<_timer.h>
#include<_textureLoader.h>
#include<_player.h>

class _enemy
{
    public:
        _enemy();
        virtual ~_enemy();

        // Timer / Texture Loader
        _timer * enemyTimer = new _timer();
        _textureLoader * enemyTextureLoader = new _textureLoader();

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
        void drawEnemy(GLuint);
        void placeEnemy(vec3);
        void enemyActions(float deltaTime);
        void takeDamage(float damage);
        enum{PURSUIT,ATTACK};

        float maxHp, currentHp;


    protected:

    private:
};

#endif // _ENEMY_H
