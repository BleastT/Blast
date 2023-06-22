#pragma once


#include <iostream>
#include <string>
#include <vector>

namespace BL
{
    struct Component
    {
        private:
            std::vector<Component> m_children;
            std::string m_name;

        public:

            Component& name (std::string new_name)
            {
                m_name = new_name;
                return *this;
            }
            Component& addChildren(std::vector<Component> children)
            {
                m_children.insert(m_children.end(), children.begin(), children.end());
                return *this;
            }

            std::vector<Component>& getChildren()
            {
                return m_children;
            } 

            std::string getName()
            {
                return m_name;
            }


            virtual void start() {}

            virtual void quit() {}

            virtual void render() {}
    };



}