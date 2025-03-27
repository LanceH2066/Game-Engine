#ifndef _SOUNDS_H
#define _SOUNDS_H

#include<SNDS/irrKlang.h>
#include<_common.h>
using namespace irrklang;

class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();

        ISoundEngine *soundEngine = createIrrKlangDevice(); // create sound engine

        void playMusic(char* fileName);     // play background music
        void playSounds(char* fileName);    // play sounds
        void pauseSounds(char* fileName);   // Pause or Stop music
        int initSounds();                   // Initialization

    protected:

    private:
};

#endif // _SOUNDS_H
