#include "FastWatermark.h"
#include "mylog.h"

FastWatermark::FastWatermark(int xPos, int yPos, const char* ttfUrl, int pix):
    m_x_pos(xPos),
    m_y_pos(yPos),
    m_ttf_url(ttfUrl),
    m_char_pixel(pix)
{
    init();
}

FastWatermark::~FastWatermark(){
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void FastWatermark::init(){
    int ret = 0;

    // Initialize FreeType library
    ret = FT_Init_FreeType(&library);
    if(ret){
        mylog(E, "FT_Init_FreeType error [%d]\n", ret);
        assert(false);
        return;
    }

    // Load a font face (replace "your_font.ttf" with your actual font file)
    ret = FT_New_Face(library, m_ttf_url, 0, &face);
    if(ret){
        mylog(E, "FT_New_Face error [%d]\n", ret);
        assert(false);
        return;
    }

    // Set font size (example: 48 pixels)
    ret = FT_Set_Pixel_Sizes(face, 0, m_char_pixel);
    if(ret){
        mylog(E, "FT_Set_Pixel_Sizes error [%d]\n", ret);
        assert(false);
        return;
    }

    preloadGlyphs();

    // Set character size (example: 48 * 64 = 3840 pixels)
    // FT_Set_Char_Size(face, 0, 48 * 64, 72, 72);
}

void FastWatermark::preloadGlyphs() {
    const char* charset = "0123456789-_";  // 你需要的字符集
    
    for (const char* p = charset; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            mylog(E, "Failed to load glyph %c", *p);
            continue;
        }
        
        FT_GlyphSlot slot = face->glyph;
        CachedGlyph glyph;
        
        // 复制所有必要数据
        glyph.width = slot->bitmap.width;
        glyph.height = slot->bitmap.rows;
        glyph.advance = slot->advance.x >> 6; // different chars may have different advance, typically it's 27 for numbers in 48px font
        glyph.bitmap_top = slot->bitmap_top;
        glyph.pitch = slot->bitmap.pitch;
        
        // 深拷贝位图数据
        size_t dataSize = glyph.height * glyph.pitch;
        glyph.bitmap.resize(dataSize);
        memcpy(glyph.bitmap.data(), slot->bitmap.buffer, dataSize);
        
        glyphCache[*p] = std::move(glyph);
    }
    
    cacheInitialized = true;
    mylog(I, "Preloaded %ld glyphs", glyphCache.size());
}


void FastWatermark::Nv12AddDateWatermark(unsigned char* nv12Buf, int width, int height, const char *text){
    unsigned int picSize = width * height * 3 / 2;
    int x = m_x_pos;

    for (const char* p = text; *p; p++) {
        // if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
        //     mylog(E, "Failed to load glyph %c", *p);
        //     continue;
        // }
        // FT_GlyphSlot slot = face->glyph;

        auto it = glyphCache.find(*p);
        if (it == glyphCache.end()) {
            mylog(E, "Glyph not found in cache: %c", *p);
            assert(false);
            continue;
        }

        const CachedGlyph& glyph = it->second;
        int y_pos = m_y_pos - glyph.bitmap_top;

        for (int i = 0; i < glyph.height; i++) {
            for (int j = 0; j < glyph.width; j++) {
                uint8_t alpha = glyph.bitmap[i * glyph.pitch + j];
                
                if (alpha > 0) {
                    uint32_t index = (y_pos + i) * width + x + j;
                    if (index >= picSize) continue;  // 边界检查
                    
                    float alpha_ratio = alpha / 255.0f;
                    nv12Buf[index] = static_cast<uint8_t>(
                        (1.0f - alpha_ratio) * nv12Buf[index] + alpha_ratio * 255
                    );
                }
            }
        }

        x += glyph.advance;
    }
}
