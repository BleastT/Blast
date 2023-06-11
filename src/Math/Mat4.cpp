#include "Mat4.hpp"


namespace BL
{
    Mat4::Mat4(float identity)
    {
        for(int i = 0; i < 16; i++)
        {
            mat[i] = 0;
        }
        mat[0 + (0 * 4)] = identity;
        mat[1 + (1 * 4)] = identity;
        mat[2 + (2 * 4)] = identity;
        mat[3 + (3 * 4)] = identity;
    }

    Mat4::Mat4()
    {
        Mat4(0.0f);
    }

    Mat4 Mat4::orthographic(float left, float right, float top, float bottom, float near, float far)
    {
        Mat4 ortho(1.0f);

        ortho.mat[0 + (0 * 4)] = 2.0f / (right - left);
        ortho.mat[3 + (0 * 4)] = -(right + left) / (right - left);
        ortho.mat[1 + (1 * 4)] = 2.0f / (top - bottom);
        ortho.mat[3 + (1 * 4)] = -(top + bottom) / (top - bottom);
        ortho.mat[2 + (2 * 4)] = -2.0f / (far - near);
        ortho.mat[3 + (2 * 4)] = -(far + near) / (far - near);

        return ortho;
    }

    Mat4 Mat4::multiply(Mat4& other)
    {
        Mat4 result(1.0f);

        for(int i = 0 ; i < 4; i ++)
        {
            for(int j = 0 ; j < 4; j++)   
            {
                float v0 = mat[0 + (i * 4)] * other.mat[j + (0 * 4)];     
                float v1 = mat[1 + (i * 4)] * other.mat[j + (1 * 4)];            
                float v2 = mat[2 + (i * 4)] * other.mat[j + (2 * 4)];            
                float v3 = mat[3 + (i * 4)] * other.mat[j + (3 * 4)];  

                result.mat[j + i * 4] = v0 + v1 + v2 + v3;
            }       
        }

        return result;
    }

    Mat4 Mat4::rotation(Vec3& rotation)
    {
        Mat4 rm(1.0f);

        float PI = 3.14159265359;

        float cosX = cos(rotation.x * PI / 180);
        float cosY = cos(rotation.y * PI / 180);
        float cosZ = cos(rotation.z * PI / 180);

        float sinX = sin(rotation.x * PI / 180);
        float sinY = sin(rotation.y * PI / 180);
        float sinZ = sin(rotation.z * PI / 180);

        rm.mat[0 + (0 * 4)] = cosY * cosZ;
        rm.mat[1 + (0 * 4)] = (sinX * sinY * cosZ) + (cosX * sinZ);
        rm.mat[2 + (0 * 4)] = -(cosX * sinY * cosZ) + (sinX * sinZ);

        rm.mat[0 + (1 * 4)] = -(cosY * sinZ);
        rm.mat[1 + (1 * 4)] = -(sinX * sinY * sinZ) + (cosX * cosZ);
        rm.mat[1 + (1 * 6)] = (cosX * sinY * sinZ) + (sinX * cosZ);

        rm.mat[0 + (2 * 4)] = sinY;
        rm.mat[1 + (2 * 4)] = -(sinX * cosY);
        rm.mat[2 + (2 * 4)] = (cosX * cosY);

        return rm;
    }

    Mat4 Mat4::scaling(Vec3& scaling)
    {
        Mat4 sm(1.0f);

        sm.mat[0 + (0 * 4)] = scaling.x;
        sm.mat[1 + (1 * 4)] = scaling.y;
        sm.mat[2 + (2 * 4)] = scaling.z;

        return sm;
    }
    
    Mat4 Mat4::translation(Vec3& translation)
    {
        Mat4 tm(1.0f);

        tm.mat[3 + (0 * 4)] = translation.x;
        tm.mat[3 + (1 * 4)] = translation.y;
        tm.mat[3 + (2 * 4)] = translation.z;

        return tm;
    }

    Mat4 Mat4::transform(Vec3& rotation, Vec3& scaling, Vec3& tranlation)
    {
        Mat4 rm = Mat4::rotation(rotation);
        Mat4 sm = Mat4::scaling(scaling);
        Mat4 tm = Mat4::translation(tranlation);

        Mat4 trm = tm.multiply(rm).multiply(sm);

        return trm;
    }
}