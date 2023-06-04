#pragma once

#include <string>
#include <iostream>
#include <cmath>

#include "./Vec3.hpp"

namespace BL
{
    struct Mat4
    {
        float mat[16];


        Mat4(float identity);
        Mat4();


        static Mat4 orthographic(float left, float right, float top, float bottom, float near, float far);

        Mat4 multiply(Mat4& other);

        static Mat4 rotation(Vec3& rotation);
        static Mat4 scaling(Vec3& scaling);
        static Mat4 translation(Vec3& translation);

        static Mat4 transform(Vec3& rotation, Vec3& scaling, Vec3& translation);

        void print(std::string name)
        {
            std::cout << "Mat4 [" << name << "]\n";

            for(int i = 0 ; i < 4; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    std::cout << mat[j + i * 4] << ",";
                }
                std::cout << "\n";
            }
        }

    };
}