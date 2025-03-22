#include "_inputs.h"

_inputs::_inputs()
{

}

_inputs::~_inputs()
{

}

void _inputs::keyPressed(_player* player, _sounds* sounds)
{
    bool moveLeft = GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000;
    bool moveRight = GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000;
    bool moveUp = GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000;
    bool moveDown = GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000;

    if (moveLeft && moveUp) player->setFacingDirection("up-left");
    else if (moveRight && moveUp) player->setFacingDirection("up-right");
    else if (moveLeft && moveDown) player->setFacingDirection("down-left");
    else if (moveRight && moveDown) player->setFacingDirection("down-right");
    else if (moveLeft) player->setFacingDirection("left");
    else if (moveRight) player->setFacingDirection("right");
    else if (moveUp) player->setFacingDirection("up");
    else if (moveDown) player->setFacingDirection("down");
    else player->actionTrigger = player->IDLE;

    if (moveLeft || moveRight || moveUp || moveDown)
    {
        sounds->playThrusterSound();
    }
}

void _inputs::keyUp(_player* player, _sounds* sounds)
{
    bool moveLeft = GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000;
    bool moveRight = GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000;
    bool moveUp = GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000;
    bool moveDown = GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000;

    if (!moveLeft && !moveRight && !moveUp && !moveDown)
    {
        player->actionTrigger = player->IDLE;
        sounds->stopThrusterSound();
    }
}

