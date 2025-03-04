#ifndef _TIMER_H
#define _TIMER_H

#include<_common.h>

class _timer
{
    public:
        _timer();
        virtual ~_timer();

        clock_t startTime;
        clock_t pauseTime;

        bool timeStart;
        bool timePaused;

        clock_t getTicks();

        void pause();
        void unPause();
        void stop();
        void reset();

    protected:

    private:
};

#endif // _TIMER_H
