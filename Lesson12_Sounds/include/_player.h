#ifndef _PLAYER_H
#define _PLAYER_H

#include<_common.h>
#include<_timer.h>
#include<_textureLoader.h>

class _player
{
    public:
        _player();
        virtual ~_player();

        _textureLoader * playerTextureLoader = new _textureLoader();
        _timer * playerTimer = new _timer();

        void initPlayer(int,int,char*,char*,char*); // number of x,y frames, filename
        void drawPlayer();              // render sprites
        void playerActions();           // decide actions
        void setFacingDirection(int direction);

        enum {IDLE,RUN,JUMP,ATTACK,WALK_BACKWARD}; // Player Actions based on sprite
        vec3 playerPosition;
        vec3 playerScale;

        int framesX;    //  number of frame columns
        int framesY;    // number of frame rows
        int currentFrame; // Keeps track of the current animation frame

        float xMax, yMax, xMin, yMin;   // texture coordinates
        vec3 vertices[4];   // draw QUAD to place player sprite

        int actionTrigger;  // select action
        int facingDirection;

        char * playerIdle;
        char * playerRun;
        char * playerAttack;

    protected:

    private:
};

#endif // _PLAYER_H
