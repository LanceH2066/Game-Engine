#ifndef _SCENE_
#define _SCENE_

#include<_common.h>
#include<_model.h>
#include<_player.h>
#include<_bullets.h>
#include<_inputs.h>
#include<_textureloader.h>
#include<_parallax.h>
#include<_sounds.h>

class _scene
{
    public:
        _scene();             //constructor
        virtual ~_scene();    //destructor

       GLint initGL();        //initialize game objects
       void reSize(GLint width,GLint Height); // resize window
       void  drawScene();     // Render the Final scene

        void spawnBullet();

       int winMsg(HWND	hWnd,			// Handle For This Window
                  UINT	uMsg,			// Message For This Window
                  WPARAM	wParam,			// Additional Message Information
                  LPARAM	lParam);

        vec2 dim; // for screen width and height;

    protected:

    private:
};

#endif // _SCENE_
