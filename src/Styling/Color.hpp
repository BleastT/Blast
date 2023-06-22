#pragma once

#include <string>

#include "../Math/Vec3.hpp"
#include "./ColorTable.hpp"

namespace BL
{
    struct Color
    {
        private:
            Vec3 m_rgb;
            Vec3 m_rgb_formated;

        public:

            static Color rgb(int r, int g, int b)
            {
                Color new_color;

                new_color.m_rgb.x = r;
                new_color.m_rgb.y = g;
                new_color.m_rgb.z = b;

                new_color.format_rgb();
                return new_color;
            }

            static Color hex(std::string hexcode)
            {
                Color new_color;

                int r, g, b;

                if(hexcode.find("#") != std::string::npos)
                    sscanf(hexcode.c_str(), "#%02x%02x%02x", &r, &g, &b);
                else 
                    sscanf(hexcode.c_str(), "%02x%02x%02x", &r, &g, &b);

                new_color.m_rgb.x = r;
                new_color.m_rgb.y = g;
                new_color.m_rgb.z = b;

                std::cout << r << " " << g << " " << b << std::endl;

                new_color.format_rgb();

                return new_color;
            }

            static Color color(std::string color)
            {
                Color new_color;

                new_color.format_rgb();
                return hex(color);
            }

            Vec3 getFormattedRGB()
            {
                return m_rgb_formated;
            }

            Vec3 getRGB()
            {
                return m_rgb;
            }

        private:
            void format_rgb()
            {
                float r = (m_rgb.x * 1.0f) / 255.0f;
                float g = (m_rgb.y * 1.0f) / 255.0f;
                float b = (m_rgb.z * 1.0f) / 255.0f;

                m_rgb_formated.x = r;
                m_rgb_formated.y = g;
                m_rgb_formated.z = b;
            }

    };
} 
