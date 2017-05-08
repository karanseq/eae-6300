Gameplay
--------
- You control a white ship that automatically shoots bullets.
- The objective of the game is to kill all the colored enemies on the screen.
- The game contains five levels, each increasing in difficulty.
- You have infinite ships but if your ship takes damage from a single enemy bullet, your level will restart.
- Completing the fifth level starts you back at the first level instead of quitting.

Instructions
------------
- Use the left and right arrow keys to move your ship.
- Hitting 'P' pauses the game.
- Hitting 'R' and 'Q' while the game is paused causes the level to restart and the game to quit respectively.

Cool Things
-----------
Job System:
    - The engine provides a job system that can have multiple job queues, each with multiple workers.
    - Each worker contains an std::thread and waits for jobs to be posted in the corresponding queue.
    - Any kind and amount of jobs can be added to the job system, as long as a given job implements InterfaceJob.
    - The final project uses jobs to load files and create actors.

Timer Events:
    - The engine has an Updater that manages broadcasting the tick to all entities that need it.
    - The Updater also maintains a list of timer events and broadcasts them.
    - A timer event contains a function object. It invokes this function object after a specified duration either once, multiple times or till it is destroyed.
    - The game uses timer events for a lot of the gameplay including player & enemy bullet fire, enemy movement and level transition effects.

Miscellaneous:
    - The game is almost entirely data driven with lua files for configuration, actors and levels. I wanted to make a configuration file for the engine as well.
    - The collision system treats static and dynamic objects differently and provides collision filters to customize collision detection.
    - The engine provides a file cache. File load jobs add files into this cache to prevent repeated reads from disk.
    - The allocators print out useful information such as their highwater mark and the number of allocations for a given size. Can be found in the output window of a debug build.
