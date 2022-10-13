#include "app.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <algorithm>

#include "const.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define STEP 0.1f
#define OBJECT_SIZE 100
#define OBJECT_HALF_SIZE OBJECT_SIZE / 2

#define PLAYER_SIZE 10
#define PLAYER_HALF_SIZE PLAYER_SIZE / 2

#define GRID 100
#define GRID_TILE 0.1f

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 3

App::App() : object(nullptr), grid(nullptr){};
SDL_Window *App::getWindow() { return this->window; }

App::~App() {
  SDL_DestroyWindow(this->window);
  this->clearObjects();
  this->models.clear();
}

void App::clearGrid() {
  GridTile *current = this->grid;
  while (current) {
    GridTile *next = current->next;
    delete current;
    current = next;
  }
}

void App::initGrid() {
  for (float x = 0.0f; x < GRID_TILE * GRID; x += GRID_TILE) {
    for (float z = 0.0f; z < GRID_TILE * GRID; z += GRID_TILE) {
      GridTile *tile = new GridTile();
      tile->next = this->grid;
      tile->a.setPosition(x, 0, z);
      tile->b.setPosition(x + GRID_TILE - 0.02f, 0, z);
      tile->c.setPosition(x + GRID_TILE - 0.02f, 0, z + GRID_TILE - 0.02f);
      tile->d.setPosition(x, 0, z + GRID_TILE - 0.02f);
      this->grid = tile;
    }
  }
}

void App::clearObjects() {
  Object *current = this->object;

  while (current) {
    Object *next = current->next;
    current->next = nullptr;
    delete current;
    current = next;
  }

  this->object = nullptr;
}

int App::initVideo(int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL init failed %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  this->window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, width, height,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (this->window == NULL) {
    SDL_Log("Create window failed %s\n", SDL_GetError());
    return 2;
  }

  SDL_GLContext context = SDL_GL_CreateContext(this->window);
  if (context == NULL) {
    SDL_Log("Create context failed\n");
    return 3;
  }

  int initGlStatus = initGL();
  if (initGlStatus != 0) {
    SDL_Log("init gl failed\n");
    return 5;
  }

  if (glewInit() != GLEW_OK) {
    SDL_Log("glew init failed\n");
    return 6;
  }

  return 0;
}

int App::initGL() {
  bool success = true;
  GLenum error = GL_NO_ERROR;

  // Initialize Projection Matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  error = glGetError();
  if (error != GL_NO_ERROR) {
    SDL_Log("Init OpenGl failed %s\n", gluErrorString(error));
    return 10;
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  error = glGetError();
  if (error != GL_NO_ERROR) {
    SDL_Log("Init OpenGl failed %s\n", gluErrorString(error));
    return 11;
  }

  error = glGetError();
  if (error != GL_NO_ERROR) {
    SDL_Log("Init OpenGl failed %s\n", gluErrorString(error));
    return 12;
  }

  return 0;
}

int App::init(int width, int height) {
  this->width = width;
  this->height = height;

  SDL_Log("init start\n");
  int errorCode = this->initVideo(width, height);

  if (errorCode == 0) {
    this->player.setPosition(0, 0, 0);
    this->quit = false;

    Model *model = new Model();
    bool result = model->load("./medivalhouse/Medieval_House.obj",
                              "./medivalhouse/Texture/Medieval_House_Diff.png");
    SDL_Log("load result: %d\n", result);
    if (result) {
      this->models.push_back(model);
      this->createObject({0, 0, 0})->model = model;
    } else {
      delete model;
    }

    this->initGrid();
    SDL_Log("init done\n");
  }
  return errorCode;
};

Object *App::createObject(Position position) {
  Object *ob = new Object;
  ob->setPosition(position);
  ob->next = this->object;
  this->object = ob;
  return ob;
}

void App::updateCreature(Creature &creature, float deltaTime) {
  if (creature.target.active) {
    creature.x = creature.x + creature.direction.x * creature.speed * deltaTime;
    creature.z = creature.z + creature.direction.z * creature.speed * deltaTime;
    float distance = creature.distance(creature.target);
    if (distance < CONST::minDistanceToMove) {
      creature.target.active = false;
      SDL_Log("stop creature, pos %f %f %f\n", creature.x, creature.y,
              creature.z);
    }
  }
}

void App::events() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      quit = true;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.scancode) {
      case SDL_SCANCODE_SPACE:
        this->enableColorMesh = !this->enableColorMesh;
        break;
      case SDL_SCANCODE_W:
        this->player.z += STEP;
        break;
      case SDL_SCANCODE_S:
        this->player.z -= STEP;
        break;
      case SDL_SCANCODE_A:
        this->player.x += STEP;
        break;
      case SDL_SCANCODE_D:
        this->player.x -= STEP;
        break;
      case SDL_SCANCODE_Q:
        this->player.moveTo(0, 0, 1);
        break;
      case SDL_SCANCODE_1:
        this->object->model->scale -= 0.002f;
        SDL_Log("scale%f\n", this->object->model->scale);
        break;
      case SDL_SCANCODE_2:
        this->object->model->scale += 0.002f;
        SDL_Log("scale%f\n", this->object->model->scale);
        break;
      case SDL_SCANCODE_3:
        this->enableObjects = !this->enableObjects;
        break;
      case SDL_SCANCODE_ESCAPE:
        this->quit = true;
        break;
      default:
        break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      switch (e.button.button) {
      case MOUSE_LEFT:
        this->mouse.setPosition(e.button.x, e.button.y, 0);
        this->click();
        break;
      }
      break;
    }
  }
}

void App::loop() {
  Uint32 lastUpdate = SDL_GetTicks();
  while (false == this->quit) {
    Uint32 currentUpdate = SDL_GetTicks();
    Uint64 currentFrameStart = SDL_GetPerformanceCounter();

    float deltaTime = (currentUpdate - lastUpdate) / 1000.0f;
    lastUpdate = currentUpdate;
    this->updateCreature(this->player, deltaTime);

    this->render();
    this->events();
    if (this->enableObjects) {
      this->renderObjects();
    }
    SDL_GL_SwapWindow(this->window);

    SDL_Delay(5);

    Uint64 currentFrameEnd = SDL_GetPerformanceCounter();
    float elapsedMS = (currentFrameEnd - currentFrameStart) /
                      (float)SDL_GetPerformanceFrequency() * 1000.0f;

    if (elapsedMS < 16.666f) {
      SDL_Delay(floor(16.666f - elapsedMS));
    }
  }
}

void App::renderGrid() {
  GridTile *current = this->grid;
  glColor3f(0.7, 0.7, 0.7);
  while (current) {
    glBegin(GL_QUADS);
    glVertex3f(current->a.x, current->a.y, current->a.z);
    glVertex3f(current->b.x, current->b.y, current->b.z);
    glVertex3f(current->c.x, current->c.y, current->c.z);
    glVertex3f(current->d.x, current->d.y, current->d.z);
    glEnd();
    current = current->next;
  }
}

void App::renderColorMesh() {}

void App::renderWorld() {
  this->renderGrid();

  glBegin(GL_QUADS);
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(this->player.x - 0.02f, this->player.y, this->player.z - 0.02f);
  glVertex3f(this->player.x + 0.02f, this->player.y, this->player.z - 0.02f);
  glVertex3f(this->player.x + 0.02f, this->player.y, this->player.z + 0.02f);
  glVertex3f(this->player.x - 0.02f, this->player.y, this->player.z + 0.02f);
  glEnd();

  glBegin(GL_LINES);

  glColor3d(1.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(1.0, 0.0, 0.0);

  glColor3d(0.0, 1.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 1.0, 0.0);

  glColor3d(0.0, 0.0, 1.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 1.0);

  glEnd();

  glPointSize(10);
  glBegin(GL_POINTS);
  // glColor3f(1, 0, 0);
  // glVertex3f(1, 0, 0);
  // glVertex3f(1, 0, 1);
  // glVertex3f(0, 0, 0);
  // glVertex3f(0, 0, 1);
  glColor3f(0, 0, 0);
  glVertex3f(this->player.x, this->player.y, this->player.z);
  glColor3f(0, 1, 0);
  glVertex3f(this->player.x, this->clickPos.y, this->player.z);
  glColor3f(1, 0, 0);
  glVertex3f(this->clickPos.x, this->clickPos.y, this->clickPos.z);
  glColor3f(0, 1, 0);
  glVertex3f(this->clickPos2.x, this->clickPos2.y, this->clickPos2.z);

  glEnd();

  glBegin(GL_LINES);
  glVertex3f(this->clickPos.x, this->clickPos.y, this->clickPos.z);
  glVertex3f(this->clickPos2.x, this->clickPos2.y, this->clickPos2.z);
  glEnd();
}

void App::click() {
  GLint view[4];
  glGetIntegerv(GL_VIEWPORT, view);
  GLdouble modelview[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  GLdouble projection[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  GLdouble objX, objY, objZ;

  GLdouble winX, winY, winZ;
  gluProject(0.0f, 0.0f, 0.0f, modelview, projection, view, &winX, &winY,
             &winZ);
  winX = this->mouse.x;
  winY = view[3] - this->mouse.y;
  // winZ = 0;
  gluUnProject(winX, winY, winZ, modelview, projection, view, &objX, &objY,
               &objZ);
  SDL_Log("click (%f, %f, %f)\n", objX, objY, objZ);
  this->clickPos.setPosition(objX, objY, objZ);

  winZ = 0;
  gluUnProject(winX, winY, winZ, modelview, projection, view, &objX, &objY,
               &objZ);
  this->clickPos2.setPosition(objX, objY, objZ);

  Position a = this->clickPos;
  Position b = this->clickPos2;

  float t = -a.y / (b.y - a.y);
  float x = a.x + t * (b.x - a.x);
  float z = a.z + t * (b.z - a.z);
  SDL_Log("moveTo (%f, %f, %f)\n", x, 0.0f, z);
  this->player.moveTo(x, 0, z);
}

void App::render() {
  // SDL_Log("render\n");
  glClearColor(CLEAR_COLOR);
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  gluPerspective(CONST::camera::fovy, this->width / this->height,
                 CONST::camera::near, CONST::camera::far);
  gluLookAt(this->player.x, CONST::camera::eyeY,
            this->player.z + CONST::camera::eyeZ, this->player.x, 0,
            this->player.z, CONST::camera::upX, CONST::camera::upY,
            CONST::camera::upZ);

  this->renderWorld();
  // if (this->enableObjects) {
  // this->renderObjects();
  //}
};

void App::renderObjects() {
  glColor3f(1, 1, 1);
  Object *current = this->object;
  while (current) {
    if (current->model) {
      current->model->render();
    }
    current = current->next;
  }
}
