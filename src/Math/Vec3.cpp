#include "./Vec3.hpp"

namespace BL
{
    Vec3::Vec3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vec3::Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
}