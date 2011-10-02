#ifndef _LIGHT_H
#define _LIGHT_H
class Light {
  protected:
    Vector3d _intensity;
  public:
    inline void getIntensity(Vector3d& in){in = _intensity;}
};

#endif //_LIGHT_H
