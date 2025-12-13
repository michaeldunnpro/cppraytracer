#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "color.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

// Forward declaration
class Light;
class Shape;

class Camera {
private:
    Point position;
    Vector orientation;

public:
    Camera(Point pos = Point(0, 0, 0), Vector ori = Vector(1, 0, 0));
    Camera(const Camera&) = default;
    ~Camera() = default;

    Point get_position() const;
    Vector get_orientation() const;

    void set_position(Point pos);
    void set_orientation(Vector ori);
};

class Screen {
private:
    float const width;
    float const length;
    int dst_cam;

public:
    Screen(float wid = 100, float len = 100, int d_c = 10);
    Screen(const Screen&) = default;
    ~Screen() = default;

    float get_width() const;
    float get_length() const;
    int get_dst_cam() const;

    /**
     * @brief Set the distance from the camera, putting the middle of
     * the screen at the point the orientation of the camera points to
     * @param d_c New distance from the screen to the camera
     */
    void set_dst_cam(int d_c);

    /**
     * @brief Take an x and y pixel as well as a pointer to a camera, returns the coordinates of that pixel
     * @param x Horizontal screen coordinate between `0` and `1`, `0` is left, `1` is right
     * @param y Vertical screen coordinate, between `0` and `1`, `0` is up, `1` is down
     * @param cam A pointer to the camera
     * @return The coordinates of the specified pixel
     */
    Point get_pixel(float x, float y, Camera* cam) const;
};

class Scene {
private:
    Camera* camera;
    Screen* screen;

    std::vector<std::unique_ptr<Shape>> shapes;
    std::vector<std::unique_ptr<Light>> lights;

    float ambient;
    float specular;
    float sp;
    Color background;
    int recursion_depth = 6;

public:
    Scene() = delete;
    Scene(Camera* cam, Screen* scr, float ambient, float specular, float sp, Color background);

    Camera* get_camera() const;
    Screen* get_screen() const;
    float get_ambient() const;
    float get_specular() const;
    float get_sp() const;
    Color get_background() const;

    void add_shape(std::unique_ptr<Shape>&& shape);
    void add_point_light(Point point, Color color = Color::white());
    void add_light(std::unique_ptr<Light>&& light);

    /**
     * @brief Render the scene and return a 2D array of colors.
     * @param width Number of pixels to render in horizontal direction
     * @param height Number of pixels to render in vertical direction
     * @return A vector of colors representing a 2D array of colors
     * with dimension (height, width). The color of `v` at index
     * (i, j) is given by `v[i * width + j]`. The returned data is
     * oriented so that
     * ```
     * (0, 0), (0, 1), ...
     * (1, 0), (1, 1), ...
     * ......
     * ```
     * gives the correct look.
     */
    std::vector<Color> render(int width, int height) const;

    /**
     * @brief Compute the first point a ray intersects among all shapes
     * @param ray The ray
     * @return A pair `(t, shape)` where `t` is the parameter indicating
     * the intersection position as in other methods, and `shape` is a
     * const lvalue reference to the shape being intersected.
     */
    std::optional<std::pair<float, std::reference_wrapper<Shape const>>> intersect_first_all(Ray const& ray) const;

    /**
     * @brief Get all point light sources visible from `point`.
     */
    std::vector<std::reference_wrapper<Light const>> get_visible_point_lights(Point const& point) const;

    /**
     * @param recursion_depth maximum recursion depth allowed; 0 for no recursion
     */
    Color trace(Ray const& ray, int recursion_depth) const;
};
