#include <array>
#include <cmath>

inline float min(float a, float b){
    if(a < b) return a;
    return b;
}

inline float max(float a, float b){
    if(a > b) return a;
    return b;
}

namespace mdcl{

//An interface for representing colour
class Color{
    public:
    virtual std::array<unsigned char, 4> getRGBA() = 0;
    virtual unsigned char red() = 0;
    virtual unsigned char green() = 0;
    virtual unsigned char blue() = 0;
    virtual unsigned char alpha() = 0;

    virtual ~Color(){}
}; //Class Color

//A class that represents colours with red, green and blue
class RGBColor : public Color{
    std::array<unsigned char, 4> _data;
    public:
    RGBColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255):
        _data({r,g,b,a}){}
    
    unsigned char red() override {return _data[0];}
    unsigned char green() override {return _data[1];}
    unsigned char blue() override {return _data[2];}
    unsigned char alpha() override {return _data[3];}
    std::array<unsigned char, 4> getRGBA() override {return _data;}
}; //Class RGBColor

//A class representing colours with a 360 degree hue, and percent saturation and brightness
class HSBColor : public Color{
    float _hue, _saturation, _brightness;
    unsigned char a;

    float chroma, X, M;

    public:
    HSBColor(float h, float s, float b, unsigned char a = 255):a(a){
        while(h >= 360) h -= 360;
        _hue = h;
        _saturation = min(max(0.0f,s),1.0f);
        _brightness = min(max(0.0f,b),1.0f);
        chroma = _saturation * _brightness;
        X = chroma * (1 - abs( fmod( _hue/60.0f, 2.0) - 1));
        M = _brightness - chroma;
    }
    
    unsigned char red() override {
        if(_hue < 60){return static_cast<unsigned char>((chroma + M)*255);}
        if(_hue < 120){return static_cast<unsigned char>((X + M)*255);}
        if(_hue < 180){return static_cast<unsigned char>(M*255);}
        if(_hue < 240){return static_cast<unsigned char>(M*255);}
        if(_hue < 300){return static_cast<unsigned char>((X + M)*255);}
        return static_cast<unsigned char>((chroma + M)*255);
    }
    unsigned char green() override {
        if(_hue < 60){return static_cast<unsigned char>((X + M)*255);}
        if(_hue < 120){return static_cast<unsigned char>((chroma + M)*255);}
        if(_hue < 180){return static_cast<unsigned char>((chroma + M)*255);}
        if(_hue < 240){return static_cast<unsigned char>((X + M)*255);}
        if(_hue < 300){return static_cast<unsigned char>(M*255);}
        return static_cast<unsigned char>(M*255);
    }
    unsigned char blue() override {
        if(_hue < 60){return static_cast<unsigned char>(M*255);}
        if(_hue < 120){return static_cast<unsigned char>(M*255);}
        if(_hue < 180){return static_cast<unsigned char>((X + M)*255);}
        if(_hue < 240){return static_cast<unsigned char>((chroma + M)*255);}
        if(_hue < 300){return static_cast<unsigned char>((chroma + M)*255);}
        return static_cast<unsigned char>((X + M)*255);
    }

    unsigned char alpha() override {return a;}

    std::array<unsigned char, 4> getRGBA() override {
        return {red(), green(), blue(), a};
    }
}; //Class HSBColor

} //namespace mdcl
