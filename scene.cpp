#include <vector>
#include <iostream>
#include <fstream>

#include "ray.hpp"
#include "vector.hpp"

Scene::Camera(Point pos, Vector ori){
  this->position = pos;
  this->orientation = ori;
}

Point Scene::Camera::get_position() const{
  return this->position
}

Vector Scene::Camera::get_orientation() const{
  return this->orientation;
}

void Scene::Camera::set_position(Point pos){
  this->position = pos;
}

void Scene::Camera::set_orientation(Vector ori){
  this->orientation = ori;
}

Scene::Screen(float wid, float len, int pix_wid, int pix_len, int d_c){
  this->width = wid;
  this->length = len;
  this->pixel_width = pix_wid;
  this->pixel_length = pix_len;
  this->dst_cam = d_c;
}

float Scene::Screen::get_width() const{
  return this->width;
}

float Scene::Screen::get_length() const{
  return this->length;
}

int Scene::Screen::get_pixel_width() const{
  return this->pixel_width;
}

int Scene::Screen::get_dst_cam() const{
  return this->dst_cam;
}

void Scene::Screen::set_dst_cam(int d_c){
  this->dst_cam = d_c;
}

Point Scene::Screen::get_pixel(int x, int y, Camera* cam) const{
  Point origin = cam->get_position() + (!(cam->get_orientation()) * this->get_dst_cam());
  //point camera is at moved by dst_cam in the direction of orientation
  x -= (this->get_pixel_width()) / 2;
  x *= this->get_width()/this->get_pixel_width();
  y -= (this->get_pixel_length()) / 2;
  y *= this->get_length()/this->get_pixel_length();

  Vector x_vector = !(cam->get_orientation()) ^ Vector(0,0,1);
  Vector y_vector = x_vector ^ cam->get_orientation();
  
  Point pixel = origin + (x * x_vector) + (y * y_vector);//move pixel by x and y
  return pixel;
}

Scene(Camera* cam, Screen* scr, float ambient, float specular, float sp, Color background){
  this->camera = cam;
  this->screen = scr;
  this->ambient = ambient;
  this->specular = specular;
  this->sp = sp;
  this->background = background;
}

~Scene(){
  delete this->camera;
  delete this->scene;
}

Camera* Scene::get_camera() const{
  return this->camera;
}

Screen* Scene::get_screen() const{
  return this->screen;
}

float Scene::get_ambient() const{
  return this->ambient;
}

float Scene::get_specular() const{
  return this->specular;
}

float Scene::get_sp() const{
  return this->sp;
}

Color Scene::get_background(){
  return this->background;
}

void make_screen(){ //for each pixel on the screen, trace a ray from the camera to that pixel
  ofstream Image("image.ppm");
  Image << "P3" << "\n"; //P3 format .ppm file
  Image << this->get_pixel_width() << " " << this->get_pixel_length() << "\n"; //tells the file the length and width of the image
  Image << "255" << "\n" //RGB values rated out of 255
  for(int i = 0; i < this->get_pixel_length(); i++){
    for(int j = 0; j < this->get_pixel_width(); j++){
      Point destination = this->get_screen()->get_pixel(j, i);
      Vector ray = destination - this->get_camera()->get_position();
      Color color = trace(ray);
      Image << color->get_r() << " " << color->get_g() << " " << color->get_b() << "\n"; 
    }
  }
  Image.close();
}

std::vector<Point> const& get_point_lights() const{
  return 0; //placeholder
}
  
Color trace(Ray const&) const{
  return 0;//placeholder
}
