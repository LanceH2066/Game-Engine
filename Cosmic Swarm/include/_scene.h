#ifndef _SCENE_H
#define _SCENE_H

#include<_lightSetting.h>
#include<_model.h>
#include<_inputs.h>
#include<_parallax.h>
#include<_collision.h>
#include<_sounds.h>
#include<_Bullet.h>
#include<_xporb.h>
#include<_enemyDrops.h>

class _scene
{
    public:

        _scene();                               // Constructor
        virtual ~_scene();                      // Destructor

        _inputs *input;
        _parallax *prlx1;
        _player *player;
        _collision *collision;
        _sounds *sounds;
        _textureLoader* xpOrbTexture;
        _textureLoader* enemyDropsMagnetTexture;
        _textureLoader* enemyDropsHealthTexture;
        _textureLoader* hudTexture; // New: HUD texture
        _textureLoader* fontTexture;

        _textureLoader* damageIconTexture;
        _textureLoader* fireRateIconTexture;
        _textureLoader* aoeSizeIconTexture;
        _textureLoader* speedIconTexture;
        _textureLoader* healthIconTexture;


        GLint initGL();                         // Initialize Game Objects
        void drawScene();                       // Render The Final Scene
        void reSize(GLint width, GLint height); // Resize Window

        void updateDeltaTime(); // New function to update delta time
        void processKeyboardInput();
        void updateEnemySpawning();
        vec2 dim; // for screen width/height
        vector<_enemy> enemies;
        //xpOrbs
        vector<_xpOrb> xpOrbs;
        vector<_enemyDrops> enemyDrops;

        float xpPickupRange = 5.0f; // Modifiable pickup range (world units)

        float bugSpawnInterval = 10.0f; // Spawn bugs every 10 seconds
        float lastBugSpawnTime = 0.0f; // Track last bug spawnF
        void spawnBugSwarm(); // New method for bug spawning
        vec3 worldMousePos; // Mouse position in world coordinates
        bool upgradeMenuActive = false; // Track upgrade menu state

        // Constants for maximum upgrades and weapons
        static const int MAX_NON_WEAPON_UPGRADES = 4; // Max 4 non-weapon upgrades
        static const int MAX_WEAPONS = 4; // Max 4 weapons
        static const int MAX_UPGRADE_LEVEL = 5; // Max level for any upgrade or weapon

       // Upgrade selection
        struct UpgradeOption {
            string name;        // e.g., "Damage", "Weapon_Laser"
            string displayText; // e.g., "Damage: Level 1 - 10% increased damage"
            int currentLevel;   // Current level of the upgrade/weapon
            bool isWeapon;      // True if it's a weapon upgrade
            WeaponType weaponType; // Relevant for weapon upgrades
        };
        vector<string> availableUpgrades = {
            "Damage", "Speed", "Health", "FireRate", "AoeSize",
            "Weapon_Default", "Weapon_Rocket", "Weapon_Laser", "Weapon_Flak"
        };
        vector<UpgradeOption> currentUpgradeOptions; // Current choices (3 options)
        void showUpgradeMenu();
        void selectUpgrade(int choice);
        bool checkMouseClickOnUpgrade(float mouseX, float mouseY); // New: Handle mouse clicks
        void renderText(string text, float x, float y, float scale, float spacing);
        void loadFontData(const string& path);

    struct FontChar {
        int id;
        int x, y;
        int width, height;
        int xoffset, yoffset;
        int xadvance;
        float u0, v0, u1, v1;
    };
    map<int, FontChar> bitmapFont;
    int fontTexWidth = 256;
    int fontTexHeight = 256;


    struct ActiveUpgrade {
        string name; // e.g., "Damage", "Speed"
        int level;   // Current level (1-5)
        bool isWeapon; // True if it's a weapon
    };
    vector<ActiveUpgrade> activeUpgrades; // List of applied upgrades

    protected:

    private:

    float elapsedTime = 0.0f;  // Tracks total run time
    float spawnInterval = 0.05f; // Initial time between spawns
    float lastSpawnTime = 0.0f; // Time since last spawn
    int maxEnemies = 100;  // Start with a limit

    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    float deltaTime;

    bool debugMode = false;  // Add this for toggling hitbox drawing
    bool isPaused = false;  // Add this for pausing
    bool gameOver = false;  // Add this for game-over state
    float damageCooldown = 0.0f;  // Cooldown timer to prevent rapid damage
};

#endif // _SCENE_H
