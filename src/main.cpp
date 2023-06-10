#include "Blast.hpp"
#include "Components/Component.hpp"

struct page1 : public BL::Component{

    int name;

    void changeName()
    {
        name = 200;

    }

    void update();

    void render()
    {
    }

};

int main()
{
    BL::App app;

    BL::WindowSettings ws;



    
    BL::Window* win1 = app.createWindow("windowA", ws);

    app.start();
    while(app.allRunning())
    {
        app.update();
    }
    app.end();
    return 0;
}