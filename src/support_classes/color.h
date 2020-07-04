#include <array>
#include <cmath>
#include <iostream>

float min(float a, float b){
    if(a < b) return a;
    return b;
}

float max(float a, float b){
    if(a > b) return a;
    return b;
}

namespace mdcl{

class Color{
    public:
    virtual std::array<unsigned char, 4> getRGBA() = 0;
    virtual unsigned char red() = 0;
    virtual unsigned char green() = 0;
    virtual unsigned char blue() = 0;
    virtual unsigned char alpha() = 0;
};

class RGBColor : public Color{
    std::array<unsigned char, 4> data;
    public:
    RGBColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255):
        data({r,g,b,a}){}
    
    unsigned char red() override {return data[0];}
    unsigned char green() override {return data[1];}
    unsigned char blue() override {return data[2];}
    unsigned char alpha() override {return data[3];}
    std::array<unsigned char, 4> getRGBA() override {return data;}
};

class HSBColor : public Color{
    float hue, saturation, brightness;
    unsigned char a;

    float chroma, X, M;

    public:
    HSBColor(float h, float s, float b, unsigned char a = 255):a(a){
        while(h >= 360) h -= 360;
        hue = h;
        saturation = min(max(0.0f,s),1.0f);
        brightness = min(max(0.0f,b),1.0f);
        chroma = saturation * brightness;
        X = chroma * (1 - abs( fmod( hue/60.0f, 2.0) - 1));
        M = brightness - chroma;

        std::cout << hue << ' ' << saturation << ' ' << brightness << ' ' << chroma << ' ' << X << ' ' << M << std::endl;
    }
    
    unsigned char red() override {
        if(hue < 60){return static_cast<unsigned char>((chroma + M)*255);}
        if(hue < 120){return static_cast<unsigned char>((X + M)*255);}
        if(hue < 180){return static_cast<unsigned char>(M*255);}
        if(hue < 240){return static_cast<unsigned char>(M*255);}
        if(hue < 300){return static_cast<unsigned char>((X + M)*255);}
        return static_cast<unsigned char>((chroma + M)*255);
    }
    unsigned char green() override {
        if(hue < 60){return static_cast<unsigned char>((X + M)*255);}
        if(hue < 120){return static_cast<unsigned char>((chroma + M)*255);}
        if(hue < 180){return static_cast<unsigned char>((chroma + M)*255);}
        if(hue < 240){return static_cast<unsigned char>((X + M)*255);}
        if(hue < 300){return static_cast<unsigned char>(M*255);}
        return static_cast<unsigned char>(M*255);
    }
    unsigned char blue() override {
        if(hue < 60){return static_cast<unsigned char>(M*255);}
        if(hue < 120){return static_cast<unsigned char>(M*255);}
        if(hue < 180){return static_cast<unsigned char>((X + M)*255);}
        if(hue < 240){return static_cast<unsigned char>((chroma + M)*255);}
        if(hue < 300){return static_cast<unsigned char>((chroma + M)*255);}
        return static_cast<unsigned char>((X + M)*255);
    }

    unsigned char alpha() override {return a;}

    std::array<unsigned char, 4> getRGBA() override {
        return {red(), green(), blue(), a};
    }
};

} //namespace mdcl
