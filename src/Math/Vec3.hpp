#pragma once

#include <iostream>
#include <string>

namespace BL
{
    struct Vec3
    {
        float x,y,z;

        Vec3(float x, float y, float z);
        Vec3();


        void print(std::string name)
        {
            std::cout << "Vec3 [" << name << "] ( " << x << " , " << y << " , " << z << " )\n";
        }
    };
}
