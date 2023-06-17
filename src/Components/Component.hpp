#pragma once


#include <iostream>
#include <string>
#include <vector>

namespace BL
{
    struct Component
    {
        private:
            std::string m_name;
        public:

            void name (std::string new_name)
            {
                m_name = new_name;
            }

            virtual void start() {}

            virtual void quit() {}

            virtual Component render();
    };


    struct ParentComponent : public Component
    {
        private:
            std::vector<Component> m_children;

        public :
            void addChildren(std::vector<Component> children)
            {
                m_children.insert(m_children.end(), children.begin(), children.end());
            }

            const std::vector<Component>& getChildren()
            {
                return m_children;
            }

            virtual void start() {}

            virtual void quit() {}

            virtual Component render();    
    };
}