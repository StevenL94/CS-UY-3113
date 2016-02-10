#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
#include <math.h>

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 640, 360);
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    GLuint texture = LoadTexture("p1_stand.png");
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
        glClearColor(1.0f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.setModelMatrix(modelMatrix);
        program.setProjectionMatrix(projectionMatrix);
        program.setViewMatrix(viewMatrix);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        
        
        modelMatrix.identity();
        modelMatrix.Translate(X, Y, 0);
//        modelMatrix.Translate(1.0f*elapsed, 0.1f*elapsed, 0);
//        modelMatrix.Scale(3.0f, 1.0f, 1.0f);
//        modelMatrix.Rotate(45.0f * (3.1415926f/180.0f));
        
        float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
