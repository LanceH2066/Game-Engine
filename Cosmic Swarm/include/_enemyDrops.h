#ifndef _ENEMYDROPS_H
#define _ENEMYDROPS_H
#include <_common.h>
#include <_textureLoader.h>

class _enemyDrops
{
    public:
        _enemyDrops();
        virtual ~_enemyDrops();

        enum dropType{MAGNET, HEALTH, SHIELD};

        //magnet
        bool magnetTriggered = false;       // Tracks if we've activated magnet
        float magnetTimer = 0.0f;
        float magnetDuration = 2.5f;

        // properties
        vec3 position;
        vec3 scale;
        bool isActive;
        float duration;
        dropType type;

        float timeAlive = 0.0f;
        float maxLifetime = 180.0f;  // time before disappearing

        //animation/texture
         float xMin, xMax, yMin, yMax;
        _textureLoader* dropTextureLoader = new _textureLoader();

        vec3 collisionBoxSize = { 0.4f, 0.4f, 1.0f };


        // Methods
        void initDrop(dropType t, const char* filePath);
        void drawDrop();
        void placeDrop(vec3 pos);
        void update(float deltaTime, vec3 playerPos, float& xpPickupRange, bool& magnetActive, float& currentHp, float maxHp);

        vec3 getCollisionBoxMin() const;
        vec3 getCollisionBoxMax() const;

    protected:

    private:
};

#endif // _ENEMYDROPS_H
