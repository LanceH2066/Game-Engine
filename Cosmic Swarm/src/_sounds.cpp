#include "_sounds.h"
using namespace irrklang;

_sounds::_sounds()
{

}
_sounds::~_sounds()
{
    soundEngine->drop();
}
int _sounds::initSounds()
{
    if(!soundEngine)
    {
        cout << "ERROR: Sound Engine Could Not Start" << endl;
        //return 0;
    }

    return 1;
}
void _sounds::playMusic(char* fileName)
{
    if (currentMusic) // Stop the previous music if playing
    {
        currentMusic->stop();
        currentMusic->drop();
    }

    currentMusic = soundEngine->play2D(fileName, true, false, true); // Store sound reference
    currentMusic->setVolume(0.05f);
}
void _sounds::stopMusic()
{
    if (currentMusic)
    {
        currentMusic->stop();
        currentMusic->drop();
        currentMusic = nullptr;
    }
}
void _sounds::playThrusterSound()
{
    if (!thrusterSound || thrusterSound->isFinished())
    {
        thrusterSound = soundEngine->play2D(thrusterSoundFilename, true, false, true); // Looping sound
        thrusterSound->setVolume(0.5f);
    }
}
void _sounds::playShootSound()
{
    if (!shootSound)
    {
        //shootSound = soundEngine->play2D(shootSoundFilename, false);
    }
    if(shootSound)
    {
        //shootSound->setVolume(0.5f);
    }
}
void _sounds::stopThrusterSound()
{
    if (thrusterSound)
    {
        thrusterSound->stop();
        thrusterSound->drop();
        thrusterSound = nullptr;
    }
}
