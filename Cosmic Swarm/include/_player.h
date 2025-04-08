#ifndef _PLAYER_H
#define _PLAYER_H

#include<_common.h>
#include<_timer.h>
#include<_textureLoader.h>
#include<_collision.h>
#include<_Bullet.h>
#include <_sounds.h>

class _player
{
    public:
        _player();
        virtual ~_player();

        _textureLoader * playerTextureLoader = new _textureLoader();
        _timer * playerTimer = new _timer();
        _collision *collision = new _collision();

        void initPlayer(int,int,char*); // number of x,y frames, filename
        void drawPlayer();              // render sprites
        void playerActions(float deltaTime);

        vector<_Bullet> bullets;  // Store active bullets
        _timer bulletTimer;  // Timer to regulate auto-firing
        void shoot(vec3 mousePos,_sounds *sounds);

        enum {IDLE,FLYING,SHOOTING}; // Player Actions based on sprite
        vec3 playerPosition;
        vec3 playerScale;
        vec3 playerRotation;

        int framesX;    //  number of frame columns
        int framesY;    // number of frame rows
        int currentFrame; // Keeps track of the current animation frame

        float xMax, yMax, xMin, yMin;   // texture coordinates
        vec3 vertices[4];   // draw QUAD to place player sprite

        int actionTrigger;  // select action
        float speed;

    protected:

    private:
};

#endif // _PLAYER_H
