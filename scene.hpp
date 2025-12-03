#pragma once

#include <optional>
#include <vector>
#include <utility>

#include <iostream>
#include <fstream>

#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"
#include "color.hpp"

class Scene{
  class Shape;
private:
    std::vector<Shape> shapes;
    std::vector<Point> point_lights;

    /**
     * @brief Compute the first point a ray intersects among all shapes
     * @param ray The ray
     * @return A pair `(t, shape)` where `t` is the parameter indicating
     * the intersection position as in other methods, and `shape` is a
     * const lvalue reference to the shape being intersected.
     */
    std::optional<std::pair<float, Shape const*>> intersect_first_all(Ray const& ray) const;

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
  float specular;
  float ambient;
  float sp;
  Color background;
  
  Camera* camera;
  Screen* screen;

public:
    Scene() = delete;
    Scene(Camera* cam, Screen* scr, float ambient, float specular, Color background);
    Scene(const Scene&);
    ~Scene();

    Camera* get_camera() const;
    Screen* get_screen() const;
    float get_ambient() const;
    float get_specular() const;
    float get_sp() const;
    void make_screen(); 
  
    /**
     * @brief Get the location of all point light sources visible from `point`.
     */
    std::vector<Point> get_visible_point_lights(Point const& point) const;

    /**
     * @param recursion_depth maximum recursion depth allowed; 0 for no recursion
     */
    Color trace(Ray const& ray, int recursion_depth) const;
};
