#ifndef __WATERMARK_H__
#define __WATERMARK_H__

#include <ft2build.h>
#include FT_FREETYPE_H

class Watermark{
public:
    Watermark(int xPos = 50, int yPos = 50,const char* ttfUrl = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf", int pix = 48);
    ~Watermark();
    void Nv12AddDateWatermark(unsigned char* nv12Buf, int width, int height, const char *text);

private:
    void init();

private:
    FT_Library library;
    FT_Face face;

    int m_x_pos;
    int m_y_pos;
    const char* m_ttf_url;
    int m_char_pixel;
};

#endif //__WATERMARK_H__