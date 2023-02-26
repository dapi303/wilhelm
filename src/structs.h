#ifndef structs__h
#define structs__h

#include <stdio.h>

#include <cmath>

#include "model.h"

struct AppWindow {
  int width;
  int height;
};

struct Position {
  float x = 0;
  float y = 0;
  float z = 0;

  void setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  void setPosition(Position &pos) {
    this->x = pos.x;
    this->y = pos.y;
    this->z = pos.z;
  }

  float distance(Position &p) {
    float w = std::abs(p.x - this->x);
    float h = std::abs(p.z - this->z);

    float distance = std::abs(float(sqrt(w * w + h * h)));
    return distance;
  }

  void direction(Position &p) {
    float distance = this->distance(p);
    this->x = (p.x - this->x) / distance;
    this->z = (p.z - this->z) / distance;
  }
};

struct Object : Position {
  Object *next = nullptr;
  float rotation = 0;
  std::shared_ptr<Model> model;
};

struct Target : Position {
  bool active = false;

  void set(const Position &position) {
    this->x = position.x;
    this->y = position.y;
    this->z = position.z;
    active = true;
  }

  void set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    active = true;
  }
};

struct Creature : Object {
  Target target;
  Position direction;
  float speed = 0.3f;

  void moveTo(float x, float y, float z) {
    target.set(x, y, z);
    direction.setPosition(*this);
    direction.direction(target);
  }

  void moveTo(Position &pos) {
    target.set(pos);
    direction.setPosition(*this);
    direction.direction(target);
  }
};

struct GridTile {
  Position a;
  Position b;
  Position c;
  Position d;
  GridTile *next = nullptr;

  void print() {
    printf("grid (%f %f) (%f %f) (%f %f) (%f %f)\n", a.x, a.y, b.x, b.y, c.x,
           c.y, d.x, d.y);
  }
};

#endif
