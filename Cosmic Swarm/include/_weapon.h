#ifndef _WEAPON_H
#define _WEAPON_H

#include "_common.h"
#include "_textureLoader.h"
#include "_timer.h"
#include "_enemy.h"

struct Weapon
{
    WeaponType type;
    int level = 1;               // Weapon level (1-5)
    float damage = 5.0f;         // Base damage
    float fireRate = 1.0f;       // Seconds between shots
    float aoeSize = 1.0f;        // AoE radius or spread angle
    float projSpeed = 30.0f;
    _timer timer;                // Cooldown timer
    shared_ptr<_textureLoader> texture;     // Texture for projectiles/effects
    bool isActive = false;       // Whether the weapon is unlocked

    // Initialize weapon
    void init(WeaponType t, shared_ptr<_textureLoader> tex, float dmg, float rate, float aoe, float speed) {
        type = t;
        texture = tex;
        damage = dmg;
        fireRate = rate;
        aoeSize = aoe;
        isActive = true;
        projSpeed = speed;
        timer.reset();
    }

    void levelUp()
    {
        level++;
        damage += damage*0.25;
        fireRate -= fireRate*0.1;
        aoeSize += aoeSize*0.25;
        projSpeed += projSpeed*0.25;
    }

    // Check if weapon can fire
    bool canFire()
    {
        return isActive && timer.getTicks() >= (fireRate * 1000.0f); // Convert fireRate to milliseconds
    }
};

#endif // _WEAPON_H
