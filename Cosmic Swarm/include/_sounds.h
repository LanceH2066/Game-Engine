#ifndef _SOUNDS_H
#define _SOUNDS_H

#include<SNDS/irrKlang.h>
#include<_common.h>
using namespace irrklang;

class _sounds
{
    public:
        irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
        irrklang::ISound* currentMusic = nullptr; // Background music
        irrklang::ISound* thrusterSound = nullptr; // Thruster sound

        _sounds();
        ~_sounds();

        int initSounds();
        void playMusic(char* fileName);
        void stopMusic();
        void playThrusterSound(char* fileName);
        void stopThrusterSound();
    protected:

    private:
};

#endif // _SOUNDS_H
