#include "Watermark.h"
#include "mylog.h"

Watermark::Watermark(int xPos, int yPos, const char* ttfUrl, int pix):
    m_x_pos(xPos),
    m_y_pos(yPos),
    m_ttf_url(ttfUrl),
    m_char_pixel(pix)
{
    init();
}

Watermark::~Watermark(){
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void Watermark::init(){
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

    // Set character size (example: 48 * 64 = 3840 pixels)
    // FT_Set_Char_Size(face, 0, 48 * 64, 72, 72);
}

void Watermark::Nv12AddDateWatermark(unsigned char* nv12Buf, int width, int height, const char *text){
    unsigned int picSize = width * height * 3 / 2;
    unsigned int index = 0;
    int text_alpha = 0;
    int x = m_x_pos;

    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) {
            mylog(E, "Failed to load glyph %c", *p);
            continue;
        }

        FT_GlyphSlot slot = face->glyph;

        // Calculate the y position for bottom alignment
        int y_pos = m_y_pos - slot->bitmap_top;// Align the bottom

        // Merge text bitmap (monochrome) onto NV12 image with alpha masking
        for (int i = 0; i < slot->bitmap.rows; i++) {
            for (int j = 0; j < slot->bitmap.width; j++) {
                text_alpha = slot->bitmap.buffer[i * slot->bitmap.pitch + j];  // Alpha value (0-255) of text pixel

                // Only overlay if text pixel is not fully transparent (adjust threshold as needed)
                // int y_pos = y + i - slot->bitmap_top;
                if (text_alpha > 0) {
                    // Overlay text pixel on Y plane
                    index = (y_pos + i) * width + x + j;
                    nv12Buf[index] = slot->bitmap.buffer[i * slot->bitmap.pitch + j];
                    assert(index < picSize);
                }
            }
        }

        x += slot->advance.x >> 6;  // Advance to the next character position
    }
}
