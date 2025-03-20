#include "_timer.h"

_timer::_timer()
{
    startTime = clock();
}

_timer::~_timer()
{

}

clock_t _timer::getTicks()
{
    return clock()-startTime;
}

void _timer::pause()
{

}

void _timer::unPause()
{

}

void _timer::stop()
{

}

void _timer::reset()
{
    startTime = clock();
}
