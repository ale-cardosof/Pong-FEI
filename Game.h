// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2
{
    float x;
    float y;
};

class Game
{
public:
    Game();
    // Initialize the game
    bool Initialize();
    // Runs the game loop until the game is over
    void RunLoop();
    // Shutdown the game
    void Shutdown();
private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;

    // Pong specific
    // Direction of paddle
    int mPaddleDir;
    // Position of paddle
    Vector2 mPaddlePos;

    int mPaddleDir2;
    // Position of paddle
    Vector2 mPaddlePos2;

    // Position of ball 1
    Vector2 mBallPos;
    // Position of ball 2
    Vector2 mBallPos2;
    // Velocity of ball 1
    Vector2 mBallVel;
    // Velocity of ball 2
    Vector2 mBallVel2;
};
