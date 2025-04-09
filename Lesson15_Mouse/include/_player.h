#ifndef _PLAYER_H
#define _PLAYER_H

#include<_common.h>
#include<_textureloader.h>
#include<_timer.h>

class _player
{
    public:
        _player();
        virtual ~_player();

        _textureLoader *pTex = new _textureLoader();
        _timer *pTmer = new _timer();

        void initPlayer(int,int,char*); // number of x,y frames, filename
        void drawPlayer();              // to render
        void playerActions();           // place actions

        enum {STAND,LEFTWALK,RIGHTWALK,RUN,JUMP,ATTACK}; // define actions based on the sprite
        vec3 plPos;  //player position
        vec3 plScl;  //player Scale

        int framesX; // number of frame columns
        int framesY; // number of frame rows

        float xMax, yMax, xMin, yMin; // texture coordinates
        vec3 vert[4];  // to draw QUAD to place player image

        int actionTrigger; // to select actions


    protected:

    private:
};

#endif // _PLAYER_H
