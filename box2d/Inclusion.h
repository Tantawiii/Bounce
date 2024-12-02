#pragma once
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include "box2d/box2d.h"

using namespace std;
using namespace sf;


// Struct for storing grid positions
struct Position {
    int row;
    int col;
};

extern char grid[10][10];
const float SCALE = 30.0f; // Box2D scale factor: 1 meter = 30 pixels
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;