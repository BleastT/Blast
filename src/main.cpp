#include "Blast.hpp"

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