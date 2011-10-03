#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vector3d direction, Vector3d intensity) :
  _direction(direction)
  {
  _intensity = intensity; 
  };

DirectionalLight::~DirectionalLight() {};
