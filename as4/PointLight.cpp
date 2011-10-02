#include "PointLight.h"

PointLight::PointLight(Vector3d position, Vector3d intensity) :
  _position(position),
  _intensity(intensity) {};

PointLight::~PointLight() {};
