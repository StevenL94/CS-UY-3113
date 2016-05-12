//
//  Level.cpp
//  Final Project
//
//  Created by Steven Lee on 5/11/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#include "Level.hpp"

void Level::readFile(std::ifstream& infile, GLuint& textureID) {
    std::string line;
    while (getline(infile, line)) {
        if (line == "[header]") {
            if(!readHeader(infile)) {
                return;
            }
        }
        else if(line == "[layer]") {
            readLayerData(infile);
        }
        else if(line == "[ObjectsLayer]") {
            readEntityData(infile);
        }
    }
}

bool Level::readHeader(std::ifstream& stream) {
    std::string line;
    mapWidth = -1;
    mapHeight = -1;
    while (getline(stream, line)) {
        if (line == "") {
            break;
        }
        std::istringstream sStream(line);
        std::string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if (key == "width") {
            mapWidth = atoi(value.c_str());
        }
        else if (key == "height") {
            mapHeight = atoi(value.c_str());
        }
    }
    if (mapWidth == -1 || mapHeight == -1) {
        return false;
    }
    else { // allocate our map data
        levelData = new unsigned char*[mapHeight];
        for (int i = 0; i < mapHeight; ++i) {
            levelData[i] = new unsigned char[mapWidth];
        }
        return true;
    }
}

bool Level::readLayerData(std::ifstream& stream) {
    std::string line;
    while (getline(stream, line)) {
        if(line == "") {
            break;
        }
        std::istringstream sStream(line);
        std::string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if (key == "data") {
            for(int y=0; y < mapHeight; y++) {
                getline(stream, line);
                std::istringstream lineStream(line);
                std::string tile;
                for (int x=0; x < mapWidth; x++) {
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if (val > 0) {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        levelData[y][x] = val-1;
                    }
                    else {
                        levelData[y][x] = 0;
                    }
                }
            }
        }
    }
    return true;
}

bool Level::readEntityData(std::ifstream& stream) {
    std::string line;
    std::string type;
    while (getline(stream, line)) {
        if (line == "") {
            break;
        }
        std::istringstream sStream(line);
        std::string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if (key == "type") {
            type = value;
        }
        else if (key == "location") {
            std::istringstream lineStream(value);
            std::string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            float placeX = atof(xPosition.c_str())/16*TILE_SIZE;
            float placeY = atof(yPosition.c_str())/16*-TILE_SIZE;
            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}

void Level::placeEntity(std::string& type, float placeX, float placeY) {
    //    SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
    if (type == "Start") {
        player.x = placeX/TILE_SIZE;
        player.y = -placeY/TILE_SIZE;
    }
    if (type == "End") {
        enemy.x = placeX/TILE_SIZE;
        enemy.y = -placeY/TILE_SIZE;
    }
    if (type == "End1") {
        key.x = placeX/TILE_SIZE;
        key.y = -placeY/TILE_SIZE;
    }
}