#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
#include <math.h>
#include <vector>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    //    Use GL_BGRA or GL_BGR for Mac
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY) {
    float u = (float)(((int)index)%spriteCountX)/(float)spriteCountX;
    float v = (float)(((int)index)/spriteCountX)/(float)spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    GLfloat texCoords[] = {
        u, v+spriteHeight,
        u+spriteWidth, v,
        u, v,
        u+spriteWidth, v,
        u, v+spriteHeight,
        u+spriteWidth, v+spriteHeight
    };
    float vertices[] = {-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,  -0.5f,
        -0.5f, 0.5f, -0.5f};
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
    float texture_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    float lastFrameTicks = 0.0f;
    float angle = 45.0f * M_PI/180.0;
    float X = 0.0;
    float Y = 0.0;
    int index = 10;
    int spriteCountX = 8;
    int spriteCountY = 4;
    const int runAnimation[] = {16,17,18,19,20};
    const int numFrames = 5;
    float animationElapsed = 0.0f;
    float framesPerSecond = 60.0f;
    int currentIndex = 0;
    unsigned score1, score2 = 0;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 640, 360);
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    GLuint text = LoadTexture("text.png");
    GLuint texture = LoadTexture("characters_3.png");
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    glUseProgram(program.programID);
//    modelMatrix.Translate(1.0f, 0, 0);
//    Additive
//    modelMatrix.Translate(1.0f, 0, 0);
//    modelMatrix.Translate(-3.0f, 0, 0);
    SDL_Event event;
    bool done = false;
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    X = 0;
                    Y = 0;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    done = true;
                }
            }
//            else if (event.type == SDL_MOUSEMOTION) {
//            float unitX = (((float)event.motion.x / 650.f)*3.554);
//            float unitY = (((float)event.motion.y / 360.0f)*3.554);
//                X = unitX;
//                Y = unitY;
//            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                float unitX = (((float)event.motion.x / 650.f)*3.554);
                float unitY = (((float)event.motion.y / 360.0f)*3.554);
                if (event.button.button == 1) {
                    X = unitX;
                    Y = unitY;
                }
            }
        }
        
        float ticks = (float)SDL_GetTicks();
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        angle += elapsed;
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_UP]) {
            Y += 1/elapsed;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            Y -= 1/elapsed;
        }
        else if(keys[SDL_SCANCODE_RIGHT]) {
            X += 1/elapsed;
        }
        else if(keys[SDL_SCANCODE_LEFT]) {
            X -= 1/elapsed;
        }
        
//        Sets color for glClear
//        glClearColor(1.0f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.setModelMatrix(modelMatrix);
        program.setProjectionMatrix(projectionMatrix);
        program.setViewMatrix(viewMatrix);
        
        modelMatrix.identity();
        DrawText(&program, text, "Hello World!", 0.15, 0);
        
        modelMatrix.identity();
        modelMatrix.Translate(X, Y, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
        animationElapsed += elapsed;
        if(animationElapsed > 1.0/framesPerSecond) {
            currentIndex++;
            animationElapsed = 0.0;
            if(currentIndex > numFrames-1) {
                currentIndex = 0;
            }
        }
        DrawSpriteSheetSprite(&program, runAnimation[currentIndex], spriteCountX, spriteCountY);
        
//        DrawSpriteSheetSprite(&program, index, spriteCountX, spriteCountY);
//        modelMatrix.identity();
//        modelMatrix.Translate(X, Y, 0);
//        modelMatrix.Translate(1.0f*elapsed, 0.1f*elapsed, 0);
//        modelMatrix.Scale(3.0f, 1.0f, 1.0f);
//        modelMatrix.Rotate(45.0f * (3.1415926f/180.0f));
        
//        float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
//        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
//        glEnableVertexAttribArray(program.positionAttribute);
//        
//        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
//        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
//        glEnableVertexAttribArray(program.texCoordAttribute);
//        
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        glDisableVertexAttribArray(program.positionAttribute);
//        glDisableVertexAttribArray(program.texCoordAttribute);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
