#include "RayTracer.h"

RayTracer::RayTracer(Scene &scene, Camera &camera ) :
  _scene(scene),
  _camera(camera) {};

void RayTracer::rayTrace() {
  Ray r;
  double t;
  Shape* s;
  Vector2d pix;
  Vector3d a,b;
  glBegin(GL_POINTS);
  vector<PointLight*> pl;
  vector<DirectionalLight*> dl; 

  _scene.getPointLights(pl);
  _scene.getDirectionalLights(dl);

  while(_camera.generateSample(r)) {
    if(_scene.intersect(r, t, &s)) {
      r.getPixel(pix);
      Vector3d ray_orig, ray_dir;
      r.getOrigin(ray_orig);
      r.getDirection(ray_dir);
      Vector3d point = ray_orig + t * ray_dir;
      Vector3d normal = s->normal(point);
      Vector3d normal_hat = normal.normalized();
      Vector3d kd, ks, ka, km;
      double sp;
      s->getKd(kd);
      s->getKs(ks);
      s->getKa(ka);
      s->getKm(km);
      s->getSp(sp);

      Vector3d intensity = Vector3d::Zero();
      Vector3d pixel_color = Vector3d::Zero();
      // Do Phong shading here
      // Loop over point lights
      for( int i = 0; i < pl.size(); i++ ) {


        Vector3d pl_pos;
        pl[i]->getPosition(pl_pos);

        Vector3d pl_color;
        pl[i]->getIntensity(pl_color);

        intensity += pl_color;

        Ray r_light(Vector2d::Zero(), point, (pl_pos-point).normalized(), 1);
        Shape *tmp;
        if (_scene.intersect(r_light,t,&tmp,s)) continue;
        Vector3d i_pl = (pl_pos - point); 

        // Diffuse light
        Vector3d i_hat_pl = i_pl.normalized();
        double i_pl_dot_n = (i_hat_pl.dot( normal_hat ));
        Vector3d diff_pl = kd.cwise() * pl_color * max(0.0, i_pl_dot_n);
        
        // Specular light 
        Vector3d r_pl = -i_hat_pl + 2 * i_pl_dot_n * normal_hat;
        Vector3d spec_pl = ks.cwise() * pl_color * pow(max(0.0, r_pl.normalized().dot( -point.normalized() )), sp);

        pixel_color += diff_pl + spec_pl;
      }

      // Loop over directional lights
      for( int i = 0; i < dl.size(); i++ ) {

        Vector3d i_dl;
        dl[i]->getDirection(i_dl);

        Vector3d dl_color;
        dl[i]->getIntensity(dl_color);
        intensity += dl_color;

        Ray r_light(Vector2d::Zero(), point, -i_dl.normalized(), 1);
        Shape *tmp;
        if (_scene.intersect(r_light,t,&tmp,s)) continue;


        // Diffuse light
        Vector3d i_hat_dl = -i_dl.normalized();
        double i_dl_dot_n = (i_hat_dl.dot( normal_hat ));
        Vector3d diff_dl = kd.cwise() * dl_color * max(0.0, i_dl_dot_n);
        
        // Specular light 
        Vector3d r_dl = -i_hat_dl + 2 * i_dl_dot_n * normal_hat;
        Vector3d spec_dl = ks.cwise() * dl_color * pow(max(0.0, r_dl.normalized().dot( -point.normalized() )), sp);

        pixel_color += diff_dl + spec_dl;
      }

      // Ambient light
      Vector3d amb = ka.cwise() * intensity;

      pixel_color += amb;
 


    setPixel(pix(0), pix(1), pixel_color(0), pixel_color(1), pixel_color(2));


    }
  }
  glEnd();
}

void RayTracer::setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x+0.5, y+0.5);
}
