#include "_inputs.h"

_inputs::_inputs()
{
    isMsRotation = false;
    isMsTranslate = false;
}

_inputs::~_inputs()
{

}

void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
    case VK_LEFT:
        mdl->rotation.y += 1.0;
        break;
    case VK_RIGHT:
        mdl->rotation.y -= 1.0;
        break;
    case VK_UP:
        mdl->rotation.x += 1.0;
        break;
    case VK_DOWN:
        mdl->rotation.x -= 1.0;
        break;
    case VK_ADD:
        mdl->pos.z += 1.0;
        break;
    case VK_SUBTRACT:
        mdl->pos.z -= 1.0;
        break;
    }
}
void _inputs::keyPressedPRLX(_parallax* PRLX)
{
    switch(wParam)
    {
    case VK_LEFT:
        PRLX->scroll("left");
        break;
    case VK_RIGHT:
        PRLX->scroll("right");
        break;
    case VK_UP:
        PRLX->scroll("up");
        break;
    case VK_DOWN:
        PRLX->scroll("down");
        break;
    }
}

void _inputs::keyPressedPlayer(_player* player)
{
    bool moveLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000);
    bool moveRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000);
    bool moveUp = (GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000);
    bool moveDown = (GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000);

    if (moveLeft && moveUp) {
        player->setFacingDirection("up-left");
    }
    else if (moveRight && moveUp) {
        player->setFacingDirection("up-right");
    }
    else if (moveLeft && moveDown) {
        player->setFacingDirection("down-left");
    }
    else if (moveRight && moveDown) {
        player->setFacingDirection("down-right");
    }
    else if (moveLeft) {
        player->setFacingDirection("left");
    }
    else if (moveRight) {
        player->setFacingDirection("right");
    }
    else if (moveUp) {
        player->setFacingDirection("up");
    }
    else if (moveDown) {
        player->setFacingDirection("down");
    }
    else {
        player->actionTrigger = player->IDLE;
        player->playerActions();
    }
}
void _inputs::keyPressedSounds(_sounds* sounds, char* fileName)
{
    switch(wParam)
    {
    case VK_LEFT:
        case 'A':
        sounds->playThrusterSound(fileName);
        break;
    case VK_RIGHT:
        case 'D':
        sounds->playThrusterSound(fileName);
    case VK_UP:
        case 'W':
        sounds->playThrusterSound(fileName);
        break;
    case VK_DOWN:
        case 'S':
        sounds->playThrusterSound(fileName);
        break;
    }
}
void _inputs::keyUpSounds(_sounds* sounds, char* fileName)
{
    switch(wParam)
    {
    case VK_LEFT:
        case 'A':
        sounds->stopThrusterSound();
        break;
    case VK_RIGHT:
        case 'D':
        sounds->stopThrusterSound();
    case VK_UP:
        case 'W':
        sounds->stopThrusterSound();
        break;
    case VK_DOWN:
        case 'S':
        sounds->stopThrusterSound();
        break;
    }
}
void _inputs::keyUpPlayer(_player* player)
{
    bool moveLeft = (GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000);
    bool moveRight = (GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000);
    bool moveUp = (GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000);
    bool moveDown = (GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000);

    if (!moveLeft && !moveRight && !moveUp && !moveDown) {
        player->actionTrigger = player->IDLE;
        player->playerActions();
    }
}

void _inputs::keyUP()
{
    switch(wParam)
    {
        default: break;
    }
}

void _inputs::mouseEventDown(_model* mdl, double x, double y)
{
    prev_msX = x;
    prev_msY = y;

    switch(wParam)
    {
    case MK_LBUTTON:
        isMsRotation = true;
        break;
    case MK_RBUTTON:
        isMsTranslate = true;
        break;
    case MK_MBUTTON:
        break;
    }
}

void _inputs::mouseEventUp()
{
    isMsRotation = false;
    isMsTranslate = false;
}

void _inputs::mouseMove(_model* mdl, double x, double y)
{
    if(isMsRotation)
    {
        mdl->rotation.y += (x-prev_msX)/3.0;    // increase x angle
        mdl->rotation.x += (y-prev_msY)/3.0;    // increase y angle
    }

    if(isMsTranslate)
    {
        mdl->pos.x += (x-prev_msX)/100.0;       // increase x position
        mdl->pos.y -= (y-prev_msY)/100.0;       // decrease y position
    }

    prev_msX = x;
    prev_msY = y;
}

void _inputs::mouseWheel(_model* mdl, double delta)
{
    mdl->pos.z += delta/100.0;
}
