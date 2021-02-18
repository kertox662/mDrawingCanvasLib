#include "canvas.h"
#include "color.h"

#include <cstdlib>

class MyApp : public mdcl::Canvas{
    // mdcl::Color* c;
    float hue;


    void init() override{
        // c = new mdcl::RGBColor(40, 50, 100);
        // hue = 0;
        hue = rand()%360;
    }

    void loop() override{
        mdcl::HSBColor c(hue, 0.5, 0.5);
        clear(&c);
        hue++;
    }

    void cleanup() override{
        // delete c;
    }
    public:
    MyApp(int width, int height, std::string title) : mdcl::Canvas::Canvas(width, height,title){}
};

int main(){
    MyApp* app = new MyApp(600,600,"Hello Canvas");
    mdcl::Canvas::startCanvas(app);
    MyApp* app2 = new MyApp(400,400,"Hello2 Canvas");
    mdcl::Canvas::startCanvas(app2);
    mdcl::Canvas::runEventLoop();
    delete app;
    delete app2;
}