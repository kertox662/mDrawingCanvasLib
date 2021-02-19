#include "canvas.h"
#include "color.h"
#include "map.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

class MyApp : public mdcl::Canvas{
    float hue;
    mdcl::Color* fill;

    void init() override{
        hue = rand()%360;
        fill = new mdcl::RGBColor(100,50,100);
        setFill(fill);
    }

    void loop() override{
        mdcl::HSBColor c(hue, 0.5, 0.5);
        clear(&c);
        hue++;
        ellipse(300,300,100,200);
    }

    void cleanup() override{
        delete fill;
    }

    public:
    MyApp(int width, int height, std::string title) : mdcl::Canvas::Canvas(width, height,title){}
};

class MyApp2 : public mdcl::Canvas{
    float phase;
    int numPoints;
    mdcl::RGBColor c;
    void init() override{
        c = mdcl::RGBColor(255);
        setFill(&c);
        c = mdcl::RGBColor(255,255,255);
    }

    void loop() override{
        clear(&c);
        float interval = getWidth() / (float) numPoints;
        for(int i = 0; i < numPoints; i++){
            float x = (i+0.5)*interval;
            float y = (sinf(mdcl::map(x,0,getWidth(), 0, 2*M_PI)+phase) + 1.2) * getHeight() * 0.4;
            circle(x,y,5);
        }
        phase += 0.03;
    }

    void cleanup() override{
    }

    public:
    MyApp2(std::string title) : mdcl::Canvas::Canvas(800, 400,title){
        phase = 0;
        numPoints = 50;
    }
};

int main(){
    MyApp* app = new MyApp(600,600,"Hello Canvas");
    mdcl::Canvas::startCanvas(app);
    MyApp2* app2 = new MyApp2("Hello2 Canvas");
    mdcl::Canvas::startCanvas(app2);
    mdcl::Canvas::runEventLoop();
    delete app;
    delete app2;
}