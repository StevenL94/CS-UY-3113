//
//  Level.hpp
//  Final Project
//
//  Created by Steven Lee on 5/11/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Entity.hpp"

#define SPRITE_COUNT_X 25
#define SPRITE_COUNT_Y 23
#define TILE_SIZE 0.16f
#define LEVEL_WIDTH 30
#define LEVEL_HEIGHT 80

class Level {
public:
    Level() {};
    ~Level(){};
    
    unsigned char** levelData;
    int mapWidth;
    int mapHeight;
    GLuint textureID;
    
    Entity player;
    Entity enemy;
    Entity key;
    
    std::vector<float> texCoordData;
    std::vector<float> vertexData;
    
    void readFile(std::ifstream& infile, GLuint& textureID);
    bool readHeader(std::ifstream& stream);
    bool readLayerData(std::ifstream& stream);
    bool readEntityData(std::ifstream& stream);
    void placeEntity(std::string& type, float placeX, float placeY);
};

#endif /* Level_hpp */
