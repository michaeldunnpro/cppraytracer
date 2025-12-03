#pragma once

#include <optional>
#include <vector>
#include <utility>


#include "ray.hpp"
#include "vector.hpp"

class Scene{
private:
  class Camera{
  private:
    Point position;
    Vector orientation;
  public:
    Camera(Point pos = Point(0, 0, 0), Vector ori = Vector(1, 0, 0));
    Camera(const Camera&);
    ~Camera() = default;
    
    Point get_position() const;
    Vector get_orientation() const;

    void set_position(Point pos);
    void set_orientation(Vector ori);
  };

  class Screen{
  private:
    float const width;
    float const length;
    int const pixel_width;
    int const pixel_length;
    int dst_cam;
  public:
    Screen(float wid = 100, float len = 100, int pix_wid = 480, int pix_len = 480, int d_c = 10);
    Screen(const Screen&);
    ~Screen() = default;
    
    float get_width() const;
    float get_length() const;
    int get_pixel_width() const;
    int get_pixel_length() const;
    int get_dst_cam() const;

    void set_dst_cam(int d_c); //sets the distance from the camera, putting the middle of the screen at the point the orientation of the camera points to
    
    Point get_pixel(int x, int y, Camera* cam) const; //takes an x and y pixel as well as a pointer to a camera, returns the coordinates of that pixel
  };
  Camera* camera;
  Screen* screen;
  
public:
  Scene() = delete;
  Scene(Camera* cam, Screen* scr);
  Screen(const Screen&);
  ~Scene();

  Camera* get_camera() const;
  Screen* get_screen() const;
  float get_ambient() const;
  float get_specular() const;
  float get_sp() const;
  void make_screen(); 
  
  std::vector<Point> const& get_point_lights() const;
  
  Color trace(Ray const&) const;
};
