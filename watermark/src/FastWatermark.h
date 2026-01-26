#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <vector>
#include <cstdint>

class FastWatermark{
public:
    FastWatermark(int xPos = 50, int yPos = 50,const char* ttfUrl = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf", int pix = 48);
    ~FastWatermark();
    void Nv12AddDateWatermark(unsigned char* nv12Buf, int width, int height, const char *text);

private:
    void init();
    void preloadGlyphs();

private:
    FT_Library library;
    FT_Face face;

    int m_x_pos;
    int m_y_pos;
    const char* m_ttf_url;
    int m_char_pixel;

    struct CachedGlyph {
        std::vector<uint8_t> bitmap;  // 复制位图数据
        int width;
        int height; 
        int advance;
        int bitmap_top;
        int pitch;
    };

    std::unordered_map<char, CachedGlyph> glyphCache;
    bool cacheInitialized = false;


};