#include <array>
#include <cmath>
#include <cstdlib>
#include <utility>

#include "../src/scene_constructor.hpp"
#include "../src/util.hpp"

/**
 * @brief A sphere colored like a soccer ball. Can't rotate.
 */
class SoccerBall : public Sphere {
public:
    SoccerBall(Point center, float radius);
    std::unique_ptr<Material> material_at(Point const&) const override;
};

SoccerBall::SoccerBall(Point center, float radius)
    : Sphere(center, radius) {
}

const float phi = (1 + std::sqrt(5.0f)) / 2; // golden ratio
// Vertices of an isocahedron
const Vector nodes[12] = {
    Vector(0, 1, phi),
    Vector(0, 1, -phi),
    Vector(0, -1, phi),
    Vector(0, -1, -phi),
    Vector(phi, 0, 1),
    Vector(-phi, 0, 1),
    Vector(phi, 0, -1),
    Vector(-phi, 0, -1),
    Vector(1, phi, 0),
    Vector(1, -phi, 0),
    Vector(-1, phi, 0),
    Vector(-1, -phi, 0)
};
// Adjacency table of vertices of an isocahedron, ordered
const std::array<int, 5> adjs[12] = {
    { 2, 4, 8, 10, 5 },
    { 3, 7, 10, 8, 6 },
    { 0, 5, 11, 9, 4 },
    { 1, 6, 9, 11, 7 },
    { 6, 8, 0, 2, 9 },
    { 7, 11, 2, 0, 10 },
    { 4, 9, 3, 1, 8 },
    { 5, 10, 1, 3, 11 },
    { 10, 0, 4, 6, 1 },
    { 11, 3, 6, 4, 2 },
    { 8, 1, 7, 5, 0 },
    { 9, 2, 5, 7, 3 }
};

std::unique_ptr<Material> SoccerBall::material_at(Point const& point) const {
    Vector coord = !(point - this->center);
    Color color = Color::white();
    for (int i = 0; i < 12; i++) {
        Vector node = !nodes[i];
        // test if `coord` is in the pentagon around `node`
        bool inside = true;
        for (int j = 0; j < 5; j++) {
            // vertices of the pentagon
            Vector v1 = !(2 * node + !nodes[adjs[i][j]]);
            Vector v2 = !(2 * node + !nodes[adjs[i][(j + 1) % 5]]);
            Vector n = v1 ^ v2;
            // if `coord` and `node` are on different sides
            // with respect to the plane containing v1, v2, and the origin
            // then the point is outside of the pentagon
            if ((n * (coord - v1)) / (n * (node - v1)) < 0) {
                inside = false;
                break;
            }
        }
        if (inside) {
            // `coord` is inside a black pentagon
            color = Color::black();
            break;
        }
    }

    return std::make_unique<PBRMaterial>(color, 0.2f, 0);
}

int main() {
    // Create scene components
    auto camera = cam(Point(0, -3, 0.5), Vector(0.0f, 1.0f, 0.0f));
    auto scr = screen(10.0f, 10.0f);
    auto scn = scene(camera, scr, 0.1f, 0.5f, 8.0f, rgb(135, 206, 235));

    // Add a soccer ball
    scn.add_shape<SoccerBall>(Point(0, 1, 1), 1);

    // Add a green plane with color pattern
    // Perlin noise
    // Based on http://eastfarthing.com/blog/2015-04-21-noise/
    int const size = 256;
    int const mask = size - 1;
    int perm[size];
    float grads_x[size], grads_y[size], jit_x[size], jit_y[size];
    for (int index = 0; index < size; ++index) {
        int other = std::rand() % (index + 1);
        if (index > other)
            perm[index] = perm[other];
        perm[other] = index;
        grads_x[index] = std::cos(2.0f * kPi * index / size);
        grads_y[index] = std::sin(2.0f * kPi * index / size);
        jit_x[index] = (std::sin(std::rand()) + 1) / 2;
        jit_y[index] = (std::sin(std::rand()) + 1) / 2;
    }
    auto f = [](float t) {
        t = std::abs(t);
        return t >= 1.0f ? 0.0f : 1.0f - (3.0f - 2.0f * t) * t * t;
    };
    auto surflet = [&](float x, float y, float grad_x, float grad_y) {
        return f(x) * f(y) * (grad_x * x + grad_y * y);
    };
    auto noise = [&](float x, float y) {
        float result = 0.0f;
        int cell_x = std::floor(x);
        int cell_y = std::floor(y);
        for (int grid_y = cell_y - 1; grid_y <= cell_y + 1; ++grid_y)
            for (int grid_x = cell_x - 1; grid_x <= cell_x + 1; ++grid_x) {
                int hash = perm[(perm[grid_x & mask] + grid_y) & mask];
                result += surflet(x - (grid_x + jit_x[hash]), y - (grid_y + jit_x[hash]),
                    grads_x[hash], grads_y[hash]);
            }
        return result;
    };
    auto pattern = [&](float a, float b) {
        float t = (noise(a * 20, b * 20) + 1) / 2;
        return std::make_unique<PBRMaterial>(rgb(0, 255, 0) * t, 0.5, 0);
    };
    scn.add_shape(std::make_unique<ParametricPlane<decltype(pattern)>>(
        Point(0.0f, 0.0f, 0.0f),
        Vector(1.0f, 0.0f, 0.0f),
        Vector(0.0f, 1.0f, 0.0f),
        pattern));

    // Add point light to the scene
    scn.add_light<InverseSquarePointLight>(Point(-1.0, -0.5, 3.0), rgb(255, 255, 251), 10);

    handle_input(scn);
    return 0;
}
