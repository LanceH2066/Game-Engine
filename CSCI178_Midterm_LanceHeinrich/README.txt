CSCI 178 - Game Development Midterm Exam
Lance Heinrich
April 01, 2025

---

Project Overview:
This project implements a simple 2D cannon shooting game using OpenGL and C++. 
The objective is to meet the midterm exam requirements by creating a cannon that rotates with mouse input, fires an animated bullet on mouse click with random velocity, 
follows a projectile path, and bounces upon hitting the ground (up to 3 times). The code builds upon provided starter files and extends functionality to fulfill all specified tasks.

---

Tasks and Approach:

1. Add a model of a cannon on the game screen
   - Implemented using the `_player` class with a cannon texture ("images/cannon.png").
   - The cannon base is rendered separately to allow the cannon to rotate without rotating the base using `_model` with "images/cannon_base.png".

2. Use mouse to rotate and set angle of the cannon (degrees between 10-80)
   - Handled in `_inputs::mouseMove`. Mouse Y-axis movement adjusts `player->plRot.z`.
   - Rotation is smoothly interpolated and clamped between 10° and 80° using `std::max` and `std::min`.
   - Sensitivity is set to 0.1 for smooth control.

3. Use mouse click to shoot and set random velocity
   - Left mouse button (`WM_LBUTTONDOWN`) triggers `_inputs::mouseEventDown`, setting the bullet's `actionTrigger` to `SHOOT`.
   - In `_bullets::fireBullet`, velocity is randomized between 4.0 and 8.0 units using `rand()`: 
     `float bulletSpeed = 4.0f + static_cast<float>(rand()) / RAND_MAX * 4.0f`.
   - Bullet direction aligns with the cannon angle using `cos` and `sin`.

4. Create an animated bullet model
   - Bullet uses "images/cannonballs.png" with two frames (0-0.5 and 0.5-1.0 texture coordinates).
   - Animation toggles every 100ms in `_bullets::bActions` using a timer, switching between frames for a simple trail effect.

5. Move the bullet on a projectile path using velocity, angle, time, etc.
   - Implemented in `_bullets::bUpdate` using physics equations:
     - Horizontal: `bPos.x += velocity.x * dt`
     - Vertical: `bPos.y += velocity.y * dt`, with `velocity.y -= g * dt` (g = 9.8).
   - Time step `dt = 0.0001f` ensures smooth motion.
   - Initial velocity is set in `fireBullet` based on cannon angle and random speed.

6. Make the bullet bounce when it collides with the ground
   - Ground level set at `y = -1.35`. When `bPos.y <= -1.35`, a bounce occurs (up to 3 times).
   - Bounce logic in `_bullets::bUpdate`:
     - Reflects `velocity.y` with 60% energy retention (`velocity.y = -velocity.y * 0.6f`).
     - Reduces `velocity.x` by 80% (`velocity.x *= 0.8f`) to simulate friction.
     - Tracks `bounceCount` and resets bullet after 3 bounces.

---

Additional Features:
- Background  layer ("images/bg.png") scaled to fit the screen for visual appeal.
- Bullet rotation adjusts dynamically based on velocity direction using `atan2`.
- Canon firing sound, and sound for the canonball bouncing
---

Running the Program:
- Unzip folder, and navigate to bin\Debug and run CSCI178_Midterm_LanceHeinrich.exe

---

Notes:
- The code builds on the provided framework, extending `_scene`, `_player`, `_bullets`, and `_inputs` classes.
- Tested for functionality: cannon rotates, bullet fires, follows a projectile arc, and bounces as required.
- Potential improvements: Finer animation control, better bullet position on spawn, sound effects, or more complex bullet models (if time permitted).

---