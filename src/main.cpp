#include "Blast.hpp"


struct page1 : public BL::Component
{
    void start()
    {
        // std::cout << "page1 start function" << std::endl;
    }

    void render()
    {
        addChildren({

            BL::Block().name("block1 block2"),

        });
    }
};

int main ()
{
    BL::App app;

    BL::WindowSettings ws;


    BL::Window* win1 = app.createWindow("windowA", ws);


    BL::StyleCollection stc;

    BL::Style st1;
    st1.width = 10;
    st1.height = 100;

    BL::Style st2;
    st2.left = 150;
    st2.top = 100;

    stc.appendNewStyle("block1", st1);
    stc.appendNewStyle("block2", st2);





    win1->appendNewStyleCollection("test_stylecollection", stc);
    win1->setStyleCollectionActive("test_stylecollection");

    win1->appendNewPage("main_page", new page1());
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