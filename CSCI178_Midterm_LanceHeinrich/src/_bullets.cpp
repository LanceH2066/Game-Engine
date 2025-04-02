#include "_bullets.h"

_bullets::_bullets()
{

}

_bullets::~_bullets()
{
    //dtor
}

void _bullets::bInit(_player *player)
{
    playerRef = player;
    bPos = player->plPos;
    initialPos = bPos; // Initialize initialPos
    bRot = player->plRot;

    bDes.x = 5;
    bDes.y = 0;
    bDes.z = player->plPos.z;

    bScale.x = 1.0;
    bScale.y = 1.0;
    bScale.z = 1.0;

    xMin = yMin = 0;
    xMax = 0.5;
    yMax = 1.0;

    bLive = false;
    actionTrigger = IDLE;
}

void _bullets::bReset(vec3 pPos)
{
    bPos = pPos;
    initialPos = pPos;
    bLive = false;
    actionTrigger = IDLE;
    bounceCount = 0;
    t = 0.0f;
}

void _bullets::bUpdate(vec3 sPos, vec3 dPos, _sounds* sounds)
{
    bActions();
    if (!bLive)
        return;

    float dt = 0.0001f; // Fixed time step
    t += dt;
    const float g = 9.8f; // Gravity constant

  // Update velocity with gravity
    velocity.y -= g * dt;

    // Update position using current velocity
    bPos.x += velocity.x * dt;
    bPos.y += velocity.y * dt;

    // Update rotation based on current velocity
    if (velocity.x != 0.0f || velocity.y != 0.0f)
    {
        float travelAngleRad = atan2(velocity.y, velocity.x);
        bRot.z = travelAngleRad * (180.0f / M_PI);
    }

    // Bounce logic
    if (bPos.y <= -1.35f && bounceCount < 3)
    {
        sounds->playSounds("sounds/impact.mp3");

        // Reflect bullet back to ground level
        bPos.y = -1.35f;

        // Simulate bounce: reverse y-velocity with damping
        velocity.y = -velocity.y * 0.6f; // Reverse and reduce (60% energy retained)
        velocity.x *= 0.8f; // Reduce x-velocity slightly (friction)

        // Increment bounce count
        bounceCount++;

        // Update rotation after bounce
        float travelAngleRad = atan2(velocity.y, velocity.x);
        bRot.z = travelAngleRad * (180.0f / M_PI);

        // Reset initial position for next arc (optional, can keep t running)
        initialPos = bPos;
        // t = 0.0f; // Uncomment if you want each bounce to reset time
    }
    else if (bPos.y <= -1.0f && bounceCount >= 3)
    {
        bLive = false;
        bReset(sPos);
    }
}

void _bullets::fireBullet()
{
    // Get the player's position (cannon base)
    bPos = playerRef->plPos;

    // Define barrel length (adjust this based on cannon size)
    float barrelLength = 0.8f * playerRef->plScl.x; // Adjust as needed

    // Convert rotation to radians
    float angleRad = playerRef->plRot.z * (M_PI / 180.0f);

    // Compute direction vector (unit vector in the direction of fire)
    vec3 cannonDirection;
    cannonDirection.x = cos(angleRad);
    cannonDirection.y = sin(angleRad);

    // **Correcting X-offset dynamically based on angle**
    float xOffsetCorrection = 0.5f * sin(angleRad);  // Fine-tune as needed

    // Adjust initial position to ensure alignment
    bPos.x += (barrelLength * cannonDirection.x) + xOffsetCorrection; // Apply correction
    bPos.y += barrelLength * cannonDirection.y;
    bPos.z = playerRef->plPos.z; // Assuming no vertical offset needed

    // **Fix the height offset based on cannon angle**
    float verticalOffset = -0.25f * sin(angleRad);  // Adjust bullet's initial height
    bPos.y += verticalOffset; // Small adjustment to prevent spawning inside cannon

    // Store initial position
    initialPos = bPos;

    // Match bullet rotation to player rotation
    bRot = playerRef->plRot;

    // Set bullet speed and velocity in the firing direction
    float bulletSpeed = 4.0f + static_cast<float>(rand()) / RAND_MAX * 4.0f; // Range: 4.0 to 8.0
    velocity.x = bulletSpeed * cannonDirection.x;
    velocity.y = bulletSpeed * cannonDirection.y;

    // Set initial rotation based on velocity
    bRot.z = atan2(velocity.y, velocity.x) * (180.0f / M_PI);

    // Reset time and bounce count
    t = 0.0f;
    bounceCount = 0;
    bLive = true;
}

void _bullets::bActions()
{
    switch(actionTrigger)
    {
        case IDLE:
            bLive = false;
            xMin = 0;
            xMax = 0.5;
            break;
        case SHOOT:
        {
            bLive = true;
            if (myTime->getTicks() > 100)
            {
                if (xMin == 0)
                {
                    xMin = 0.5;
                    xMax = 1.0;
                }
                else
                {
                    xMin = 0;
                    xMax = 0.5;
                }
                myTime->reset();
            }
            break;
        }
        case HIT:
            bLive = false;
            break;
    }
}

void _bullets::drawBullet(GLuint TX)
{
    glPushMatrix();
    if (bLive)
    {
        glTranslatef(bPos.x, bPos.y, bPos.z);
        glScalef(bScale.x, bScale.y, bScale.z);

        glRotatef(bRot.x, 1, 0, 0);
        glRotatef(bRot.y, 0, 1, 0);
        glRotatef(bRot.z, 0, 0, 1);

        glBindTexture(GL_TEXTURE_2D, TX);

        glBegin(GL_QUADS);
        glTexCoord2f(xMin, yMax);
        glVertex3f(-1, -1, 0);
        glTexCoord2f(xMax, yMax);
        glVertex3f(1, -1, 0);
        glTexCoord2f(xMax, yMin);
        glVertex3f(1, 1, 0);
        glTexCoord2f(xMin, yMin);
        glVertex3f(-1, 1, 0);
        glEnd();
    }
    glPopMatrix();
}
