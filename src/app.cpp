#include "app.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <algorithm>

#include "const.h"
#include "exit_codes.h"

#define STEP 0.1f
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 3

App::App() : object(nullptr), hud(fps){};

App::~App() {
  SDL_DestroyWindow(window);
  clearObjects();
  models.clear();
}

void App::clearObjects() {
  Object *current = object;

  while (current) {
    Object *next = current->next;
    current->next = nullptr;
    delete current;
    current = next;
  }

  object = nullptr;
}

int App::initVideo(int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL init failed %s\n", SDL_GetError());
    return ERROR_INIT_SDL;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (window == NULL) {
    SDL_Log("Create window failed %s\n", SDL_GetError());
    return ERROR_CREATE_WINDOW;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == NULL) {
    SDL_Log("Create context failed\n");
    return ERROR_CREATE_CONTEXT;
  }

  int initGlStatus = initGL();
  if (initGlStatus != 0) {
    SDL_Log("init gl failed\n");
    return ERROR_INIT_GL;
  }

  if (glewInit() != GLEW_OK) {
    SDL_Log("glew init failed\n");
    return ERROR_INIT_GLEW;
  }

  return 0;
}

int App::initGL() {
  bool success = true;
  GLenum error = GL_NO_ERROR;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  error = glGetError();
  if (error != GL_NO_ERROR) {
    SDL_Log("Init OpenGl failed %s\n", gluErrorString(error));
    return ERROR_INIT_OPENGL;
  }

  error = glGetError();
  if (error != GL_NO_ERROR) {
    SDL_Log("Init OpenGl failed %s\n", gluErrorString(error));
    return ERROR_INIT_OPENGL;
  }

  error = glGetError();
  if (error != GL_NO_ERROR) {
    SDL_Log("Init OpenGl failed %s\n", gluErrorString(error));
    return ERROR_INIT_OPENGL;
  }

  return 0;
}

int App::init(int width, int height) {
  appWindow.width = width;
  appWindow.height = height;

  SDL_Log("init start\n");
  int errorCode = initVideo(width, height);

  hud.init(appWindow);

  if (errorCode == 0) {
    player.setPosition(0, 0, 0);
    quit = false;

    Model *model = new Model();
    bool result =
        model->load("./media/medivalhouse/Medieval_House.obj",
                    "./media/medivalhouse/Texture/Medieval_House_Diff.png");
    SDL_Log("load result: %d\n", result);
    if (result) {
      models.push_back(model);
      createObject({0, 0, 0})->model = model;
    } else {
      delete model;
    }

    SDL_Log("init done\n");
  }
  return errorCode;
};

Object *App::createObject(Position position) {
  Object *ob = new Object;
  ob->setPosition(position);
  ob->next = object;
  object = ob;
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
        break;
      case SDL_SCANCODE_W:
        player.z += STEP;
        break;
      case SDL_SCANCODE_S:
        player.z -= STEP;
        break;
      case SDL_SCANCODE_A:
        player.x += STEP;
        break;
      case SDL_SCANCODE_D:
        player.x -= STEP;
        break;
      case SDL_SCANCODE_Q:
        player.moveTo(0, 0, 1);
        break;
      case SDL_SCANCODE_1:
        object->model->scale -= 0.002f;
        SDL_Log("scale%f\n", object->model->scale);
        break;
      case SDL_SCANCODE_2:
        object->model->scale += 0.002f;
        SDL_Log("scale%f\n", object->model->scale);
        break;
      case SDL_SCANCODE_3:
        break;
      case SDL_SCANCODE_ESCAPE:
        quit = true;
        break;
      default:
        break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      switch (e.button.button) {
      case MOUSE_LEFT:
        mouse.setPosition(e.button.x, e.button.y, 0);
        click();
        break;
      }
      break;
    }
  }
}

void App::loop() {
  fps.init();
  while (false == quit) {
    fps.frameStart();
    updateCreature(player, fps.getDeltaTime());

    glClearColor(CLEAR_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    events();

    glUseProgram(0);
    renderScene();
    hud.render();

    SDL_GL_SwapWindow(window);

    SDL_Delay(5);

    fps.frameEnd();
  }
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
  winX = mouse.x;
  winY = view[3] - mouse.y;
  // winZ = 0;
  gluUnProject(winX, winY, winZ, modelview, projection, view, &objX, &objY,
               &objZ);
  SDL_Log("click (%f, %f, %f)\n", objX, objY, objZ);
  clickPos.setPosition(objX, objY, objZ);

  winZ = 0;
  gluUnProject(winX, winY, winZ, modelview, projection, view, &objX, &objY,
               &objZ);
  clickPos2.setPosition(objX, objY, objZ);

  Position a = clickPos;
  Position b = clickPos2;

  float t = -a.y / (b.y - a.y);
  float x = a.x + t * (b.x - a.x);
  float z = a.z + t * (b.z - a.z);
  SDL_Log("moveTo (%f, %f, %f)\n", x, 0.0f, z);
  player.moveTo(x, 0, z);
}

void App::renderScene() {
  glUseProgram(0);

  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluPerspective(CONST::camera::fovy, appWindow.width / appWindow.height,
                 CONST::camera::near, CONST::camera::far);
  gluLookAt(player.x, CONST::camera::eyeY, player.z + CONST::camera::eyeZ,
            player.x, 0, player.z, CONST::camera::upX, CONST::camera::upY,
            CONST::camera::upZ);

  renderObjects();
};

void App::renderObjects() {
  glColor3f(1, 1, 1);
  Object *current = object;
  while (current) {
    if (current->model) {
      current->model->render();
    }
    current = current->next;
  }
}
