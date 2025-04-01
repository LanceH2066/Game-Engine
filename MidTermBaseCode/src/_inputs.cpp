#include "_inputs.h"

_inputs::_inputs()
{
    //ctor
    isMsRotation = true;
    isMsTranslate = false;
}

_inputs::~_inputs()
{
    //dtor
}

void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
    case VK_LEFT:
        mdl->rotation.y +=1.0;
        break;

    case VK_RIGHT:
        mdl->rotation.y -=1.0;
        break;

    case VK_UP:
        mdl->rotation.x +=1.0;
        break;

    case VK_DOWN:
        mdl->rotation.x -=1.0;
        break;
    }
}

void _inputs::keyPressedPRLX(_parallax* PLRX)
{
     switch(wParam)
    {
    case VK_LEFT:
       PLRX->scroll(false,"left",PLRX->speed);
        break;

    case VK_RIGHT:
        PLRX->scroll(false,"right",PLRX->speed);
        break;

    case VK_UP:
        PLRX->scroll(false,"up",PLRX->speed);
        break;

    case VK_DOWN:
        PLRX->scroll(false,"down",PLRX->speed);
        break;
    }
}


void _inputs::keyPressed(_bullets &B)
{
    switch(wParam)
    {
    case VK_SPACE:
      //B.actionTrigger= B.SHOOT;
    break;
    }
}

void _inputs::keyUP()
{
    switch(wParam)
    {
        default: break;
    }

}

void _inputs::mouseEventDown(_player* player,_bullets &B)
{
    player->actionTrigger = player->FIRE;
    B.actionTrigger= B.SHOOT;
}

void _inputs::mouseEventUp(_player* player)
{
    player->actionTrigger = player->AIM;
}

void _inputs::mouseMove(_player* player, double x, double y)
{
    float sensitivity = 0.1f; // Adjust this for smoothness
    float deltaY = prev_msY- y; // Use Y-axis for rotation

    player->plRot.z += deltaY * sensitivity;

    // Smooth interpolation instead of hard clamping
    player->plRot.z = std::max(10.0f, std::min(player->plRot.z, 80.0f));

    prev_msX = x;
    prev_msY = y;
}

void _inputs::mouseWheel(_model* mdl, double delta)
{
    mdl->pos.z +=delta/100;
}
