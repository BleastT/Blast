#include "Blast.hpp"


int main ()
{
    BL::App app;

    BL::WindowSettings ws;


    BL::Window* win1 = app.createWindow("windowA", ws);


    BL::StyleCollection stc;

    BL::Style st;
    st.width = 10;
    st.height = 100;

    stc.appendNewStyle("test_style", st);

    BL::Block cp;

    win1->appendNewStyleCollection("test_stylecollection", stc);
    win1->appendNewPage("main_page", cp);
    win1->setPageActive("main_page");
    // BL::Window* win2 = app.createWindow("windowB", ws);
    // BL::Window* win3 = app.createWindow("windowC", ws);

    app.start();

    while(app.allRunning())
    {
        app.update();
    }
    app.end();
    return 0;
}