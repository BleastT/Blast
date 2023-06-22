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

            BL::Block().name("block1"),
            BL::Block().name("block2"),
            BL::Block().name("block3"),
            BL::Block().name("block4"),


            BL::Block().name("block5"),
            BL::Block().name("block6"),
            BL::Block().name("block7"),
            BL::Block().name("block8"),

            BL::Block().name("block9"),

        });
    }
};

int main ()
{
    BL::App app;

    BL::WindowSettings ws;


    BL::Window* win1 = app.createWindow("windowA", ws);


    BL::StyleCollection stc;

    BL::Style st;
    st.width = 10;
    st.height = 100;
    st.left = 150;
    st.top = 100;
    BL::Style st2;
    st2.width = 100;
    st2.height = 10;
    st2.left = 150;
    st2.top = 100;
    BL::Style st3;
    st3.width = 10;
    st3.height = 100;
    st3.left = 240;
    st3.top = 100;
    BL::Style st4;
    st4.width = 100;
    st4.height = 10;
    st4.left = 150;
    st4.top = 200;



    BL::Style st5;
    st5.width = 10;
    st5.height = 100;
    st5.left = 650;
    st5.top = 100;
    BL::Style st6;
    st6.width = 100;
    st6.height = 10;
    st6.left = 650;
    st6.top = 100;
    BL::Style st7;
    st7.width = 10;
    st7.height = 100;
    st7.left = 740;
    st7.top = 100;
    BL::Style st8;
    st8.width = 100;
    st8.height = 10;
    st8.left = 650;
    st8.top = 200;


    BL::Style st9;
    st9.background_color = BL::Color::color(BL_DARKORANGE);
    st9.width = 540;
    st9.height = 60;
    st9.left = 190;
    st9.top = 300;

    stc.appendNewStyle("block1", st);
    stc.appendNewStyle("block2", st2);
    stc.appendNewStyle("block3", st3);
    stc.appendNewStyle("block4", st4);
    stc.appendNewStyle("block5", st5);
    stc.appendNewStyle("block6", st6);
    stc.appendNewStyle("block7", st7);
    stc.appendNewStyle("block8", st8);
    stc.appendNewStyle("block9", st9);

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