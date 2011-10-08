#include "RayTracer.h"

RayTracer::RayTracer(Scene &scene, Camera &camera, int depth ) :
  _scene(scene),
  _camera(camera),
  _max_depth(depth)
{
  _scene.getPointLights(pl);
  _scene.getDirectionalLights(dl);
  camera.getViewport(v);
  
  for( int i = 0; i < v.w; i++ ) {
    vector<Vector3d> vec;
    for( int j = 0; j < v.h; j++ ) {
      vec.push_back(Vector3d::Zero());
    }
    pixel_colors.push_back(vec);
  }
};

void RayTracer::traceRay(Ray &r) {
  double t;
  Shape* s;
  Vector2d pix;
  Vector3d a,b;
  
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

      // Intensity must be added before checking for intersection
      // for ambient term
      intensity += pl_color;

      Ray r_light(Vector2d::Zero(), point, (pl_pos-point).normalized(), 1, s);
      Shape *tmp;
      if (_scene.intersect(r_light,t,&tmp)) continue;
      Vector3d i_pl = (pl_pos - point); 

      // Diffuse light
      Vector3d i_hat_pl = i_pl.normalized();
      double i_pl_dot_n = (i_hat_pl.dot( normal_hat ));
      Vector3d diff_pl = kd.cwise() * pl_color * max(0.0, i_pl_dot_n);

      // Specular light 
      Vector3d r_pl = -i_hat_pl + 2 * i_pl_dot_n * normal_hat;
      Vector3d spec_pl = ks.cwise() * pl_color * pow(max(0.0, r_pl.normalized().dot( (ray_orig-point).normalized() )), sp);

      pixel_color += diff_pl + spec_pl;
    } // for (point lights)

    // Loop over directional lights
    for( int i = 0; i < dl.size(); i++ ) {

      Vector3d i_dl;
      dl[i]->getDirection(i_dl);

      Vector3d dl_color;
      dl[i]->getIntensity(dl_color);
      // Intensity must be added before checking for intersection
      // for ambient term
      intensity += dl_color;

      Ray r_light(Vector2d::Zero(), point, -i_dl.normalized(), 1, s);
      Shape *tmp;
      if (_scene.intersect(r_light,t,&tmp)) continue;

      // Diffuse light
      Vector3d i_hat_dl = -i_dl.normalized();
      double i_dl_dot_n = (i_hat_dl.dot( normal_hat ));
      Vector3d diff_dl = kd.cwise() * dl_color * max(0.0, i_dl_dot_n);

      // Specular light 
      Vector3d r_dl = -i_hat_dl + 2 * i_dl_dot_n * normal_hat;
      Vector3d spec_dl = ks.cwise() * dl_color * pow(max(0.0, r_dl.normalized().dot( (ray_orig-point).normalized() )), sp);

      pixel_color += diff_dl + spec_dl;
    } // for (directional lights)

    Vector3d r_scale;
    r.getScale(r_scale);

    // Ambient light
    //if (r.getDepth() == 0) {
    Vector3d amb = ka.cwise() * intensity;
    pixel_color += amb;
    //}
      
#if 1
    // Add reflection ray to queue
    if( r.getDepth() < _max_depth ) {
      Vector3d ray_dir_hat = -ray_dir.normalized();
      double ray_dir_dot_n = ray_dir_hat.dot(normal_hat);  
      Vector3d reflect_dir = -ray_dir_hat + 2 * ray_dir_dot_n * normal_hat;
      //cout << "Pixel: " << pix.transpose() << " Reflecting ray origin: " << point.transpose() << " direction: " << reflect_dir.transpose() << " normal: " << normal_hat.transpose() << endl;
      //exit(0);
      Ray reflect_ray = Ray(pix, point, reflect_dir, r.getDepth() + 1, km.cwise() * r_scale, s);
      rayQueue.push(reflect_ray);
    }
#endif

    //cout << pixel_color.transpose() << endl;
    //cout << r_scale.transpose() << endl;
    //cout << pix.transpose() << endl;
    pixel_colors[pix(0)][pix(1)] += (r_scale.cwise() * pixel_color);
    //cout << "done set" << endl;
  }
}

void RayTracer::generateRays() {
  Ray r;

  while(_camera.generateSample(r)) {
    Vector3d ray_dir, ray_origin;
    r.getDirection(ray_dir);
    r.getOrigin(ray_origin);
   // cout << "Direction: " << ray_dir.transpose() << " Origin: " << ray_origin.transpose() << endl;
    rayQueue.push(r);
  }

  while(!rayQueue.empty()) {
    r = rayQueue.front();
    traceRay(r);
    rayQueue.pop();
  }
  
  glBegin(GL_POINTS);
  for( int i = 0; i < v.w; i++ ) {
    for( int j = 0; j < v.h; j++ ) {
      Vector3d rgb = pixel_colors[i][j];
      setPixel(i, j, rgb(0), rgb(1), rgb(2));
    }
  }
  glEnd();
}

void RayTracer::setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x+0.5, y+0.5);
}
