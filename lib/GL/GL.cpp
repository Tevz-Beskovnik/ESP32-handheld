#include <GL.hpp>
#include <glcdfont.c>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b)                                                   \
  {                                                                            \
    uint16_t t = a;                                                            \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

/**
 * @brief Prepares the graphics library class
 * 
 * @param clk number of pin that is connected to CLK
 * 
 * @param mosi number of pin that is connected to DI
 * 
 * @param cs number of pin that is connectec to CS
 * 
 * @param width width of connected display
 * 
 * @param height height of connected display
 * 
 * @param freq frequency of SPI, not needed in most cases
*/
GL::GL(uint8_t clk, uint8_t mosi, uint8_t cs, uint16_t width, uint16_t height, uint32_t freq)
: Display(cs, clk, mosi, freq, width, height), texture_buffer(NULL), _w(width), _h(height), _cp437(false), wrap(false)
{
  tex[0] = NULL;
  tex[1] = NULL;
  tex[2] = NULL;
  tex[3] = NULL;
  tex[4] = NULL;
  tex[5] = NULL;
  tex[6] = NULL;
  tex[7] = NULL;
}

GL::~GL()
{
  if(tex[0] != NULL)
    free(tex[0]);
  if(tex[1] != NULL)
    free(tex[1]);
  if(tex[2] != NULL)
    free(tex[2]);
  if(tex[3] != NULL)
    free(tex[3]);
  if(tex[4] != NULL)
    free(tex[4]);
  if(tex[5] != NULL)
    free(tex[5]);
  if(tex[6] != NULL)
    free(tex[6]);
  if(tex[7] != NULL)
    free(tex[7]);
  if(texture_buffer != NULL)
    free(texture_buffer);
}

/**
 * @brief initializes the graphics library
*/
void GL::initGL()
{
  this->begin();
  context_buffer = (uint8_t*)getDisplayBuffer();
}

void GL::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y) {
  if ((x >= _w) ||              // Clip right
      (y >= _h) ||             // Clip bottom
      ((x + 6 * size_x - 1) < 0) || // Clip left
      ((y + 8 * size_y - 1) < 0))   // Clip top
    return;

  if (!_cp437 && (c >= 176))
    c++; // Handle 'classic' charset behavior

  for (int8_t i = 0; i < 5; i++) 
  { // Char bitmap = 5 columns
    uint8_t line = pgm_read_byte(&font[c * 5 + i]);
    for (int8_t j = 0; j < 8; j++, line >>= 1) 
    {
      if (line & 1) 
      {
        if (size_x == 1 && size_y == 1)
          drawPixel(x + i, y + j, color);
        else
          fillRect(x + i * size_x, y + j * size_y, size_x, size_y,
                        color);
      }
      else if (bg != color)
      {
        if (size_x == 1 && size_y == 1)
          drawPixel(x + i, y + j, bg);
        else
          fillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
      }
    }
  }
  if (bg != color) { // If opaque, draw vertical line for last column
    if (size_x == 1 && size_y == 1)
      drawFastRawVLine(x + 5, y, 8, bg);
    else
      fillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
  }
}

size_t GL::write(uint8_t c) {
  if (c == '\n') {              // Newline?
    _cursor_x = 0;               // Reset x to zero,
    _cursor_y += _font_size * 8; // advance y one line
  } else if (c != '\r') {       // Ignore carriage returns
    if (wrap && ((_cursor_x + _font_size * 6) > _w)) { // Off right?
      _cursor_x = 0;                                       // Reset x to zero,
      _cursor_y += _font_size * 8; // advance y one line
    }
    drawChar(_cursor_x, _cursor_y, c, _text_color, _text_bg_color, _font_size,
             _font_size);
    _cursor_x += _font_size * 6; // Advance x one char
  }
  return 1;
}

const uint8_t PROGMEM lineCap[7] = {127, 63, 31, 15, 7, 3, 1};

void GL::drawFastRawHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  uint8_t leftoverStart = x % 8;
  uint8_t* begin = context_buffer + (y * (_w / 8) + (x)/8);
  if(8-leftoverStart > w)
  {
    if(color == WHITE)
      *begin |= (~lineCap[7-leftoverStart] & (lineCap[(7-leftoverStart-w)]));
    else
      *begin &= (lineCap[7-leftoverStart] | (~lineCap[(7-(leftoverStart-w))]));
    return;
  }

  if(color == WHITE)
    *begin |= ~lineCap[7-leftoverStart];
  else
    *begin &= lineCap[7-leftoverStart];
  w -= 8-leftoverStart;
  uint8_t secondLast = w/8;

  while(w/8 != 0)
  {
    if(color == WHITE)
      *(uint8_t*)(begin + (w/8)) |= 255;
    else
      *(uint8_t*)(begin + (w/8)) &= 0;
    w -= 8;
  }

  if(w == 0)
      return;

  if(color == WHITE)
    *(uint8_t*)(begin + secondLast + 1) |= lineCap[7-w];
  else
    *(uint8_t*)(begin + secondLast + 1) &= ~lineCap[7-w];
}

void GL::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep)
   {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) 
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) 
  {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) 
  {
    if (steep) 
    {
      drawPixel(y0, x0, color);
    } 
    else 
    {
      drawPixel(x0, y0, color);
    }

    err -= dy;
    if (err < 0) 
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void GL::drawFastRawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{
  // x & y already in raw (rotation 0) coordinates, no need to transform.
  int16_t row_bytes = ((_w + 7) / 8);
  uint8_t *ptr = &context_buffer[(x / 8) + y * row_bytes];

  if (color > 0)
  {
    uint8_t bit_mask = (0x80 >> (x & 7));

    for (int16_t i = 0; i < h; i++) {
      *ptr |= bit_mask;
      ptr += row_bytes;
    }
  } 
  else 
  {
    uint8_t bit_mask = ~(0x80 >> (x & 7));

    for (int16_t i = 0; i < h; i++) 
    {
      *ptr &= bit_mask;
      ptr += row_bytes;
    }
  }
}

void GL::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
  drawLine(x, y, x + w -1, y, color);
  drawLine(x, y + h - 1, x + w, y + h - 1, color);
  drawLine(x, y, x, y + h - 1, color);
  drawLine(x + w - 1, y, x + w - 1, y + h - 1, color);
}

void GL::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
  for (int16_t i = y; i < y + h; i++) 
  {
    //drawLine(x, i, x+w, i, color);
    drawFastRawHLine(x, i, w, color);
  }
}

void GL::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

void GL::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    drawLine(a, y0, b + 1, y0, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if (a > b)
      _swap_int16_t(a, b);
    drawLine(a, y, b + 1, y, color);
  }

  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b)
      _swap_int16_t(a, b);
    drawLine(a, y, b + 1, y, color);
  }
}

/**
 * @brief draw a circle
 * 
 * @param x coordinate of the center of the circle
 * 
 * @param y coordinate of the center of the circle
 * 
 * @param r radius of the circle
 * 
 * @param color for the circle to be drawn in
*/
void GL::drawCircle(uint16_t x, uint16_t y, uint8_t r, uint8_t color)
{
  for(uint16_t i = x; i < x+(r/4*3); i++)
  {
    uint16_t y0 = (2*y + sqrtf((4*y*y) - 4*(i*i-2*i*x+x*x+y*y-r*r)))/2;
    drawPixel(i, y0, color);  // <---- right half drawing
    drawPixel(i, y-abs(y0-y), color);
    drawPixel(x+abs(y0-y), y+abs(i-x), color);
    drawPixel(x+abs(y0-y), y-abs(i-x), color);
    drawPixel(x-abs(i-x), y0, color);  // <---- left half drawing
    drawPixel(x-abs(i-x), y-abs(y0-y), color);
    drawPixel(x-abs(y0-y), y+abs(i-x), color);
    drawPixel(x-abs(y0-y), y-abs(i-x), color);
  }
}

/**
 * @brief draws a filled circle
 * 
 * @param x coordinate of the center of the circle
 * 
 * @param y coordinate of the center of the circle
 * 
 * @param r radius of the circle
 * 
 * @param color for the circle to be drawn in
*/
void GL::fillCircle(uint16_t x, uint16_t y, uint8_t r, uint8_t color)
{
  for(uint16_t i = x; i < x+(r/4*3); i++)
  {
    uint16_t y0 = (2*y + sqrtf((4*y*y) - 4*(i*i-2*i*x+x*x+y*y-r*r)))/2;
    drawLine(x-abs(y0-y), y+abs(i-x), x-abs(y0-y), y-abs(i-x), color);
    drawLine(x-abs(i-x), y-abs(y0-y), x-abs(i-x), y0, color);
    drawLine(x+abs(y0-y), y+abs(i-x), x+abs(y0-y), y-abs(i-x), color);
    drawLine(i, y0, i, y-abs(y0-y), color);
  }
}

/**
 * @brief draws a dittered line
 * 
 * @param x0 first x coordinate
 *
 * @param y0 first y coordinate
 * 
 * @param x1 second x coordinate
 * 
 * @param y1 second y cooridnate
*/
void GL::drawLineD(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, x0%2);
    } else {
      drawPixel(x0, y0, x0%2);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

/**
 * @brief draws a dittered line with inverted ditter patern
 * 
 * @param x0 first x coordinate
 *
 * @param y0 first y coordinate
 * 
 * @param x1 second x coordinate
 * 
 * @param y1 second y cooridnate
*/
void GL::drawLineDI(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, (x0+1)%2);
    } else {
      drawPixel(x0, y0, (x0+1)%2);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

/**
 * @brief draws a dittered rectangle. Looks slighly brighter when displayed
 * 
 * @param x x cooridnate of top left corner of rectangle
 * 
 * @param y y cooridnate of top left corner of rectangle
 * 
 * @param width width of rectangle
 * 
 * @param height height of rectangle
*/
void GL::drawRectD(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  drawLineD(x, y, x+w, y);
  drawLineD(x, y+h, x+w, y+h);
  drawLineD(x, y, x, y+h);
  drawLineD(x+w, y, x+w, y+h);
}

/**
 * @brief draws a filled rectangle
 * 
 * @param x x cooridnate of top left corner of rectangle
 * 
 * @param y y cooridnate of top left corner of rectangle
 * 
 * @param width width of rectangle
 * 
 * @param height height of rectangle
*/
void GL::fillRectD(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  for(uint16_t y2 = y; y2 < y+h; y2++)
  {
    if(y2%2 == 0)
      drawLineD(x, y2, x+w, y2);
    else
      drawLineDI(x, y2, x+w, y2);
  }
}

/**
 * @brief draws a dittered triangle
 * 
 * @param x1 first x coordinate
 * 
 * @param y1 first y coordinate
 * 
 * @param x2 second x coordinate
 * 
 * @param y2 second y coordinate
 * 
 * @param x3 third x coordinate
 * 
 * @param y3 third x coordinate
*/
void GL::drawTriangleD(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
  drawLineD(x1, y1, x2, y2);
  drawLineD(x2, y2, x3, y3);
  drawLineD(x3, y3, x1, y1);
}

/**
 * @brief draws a filled dittered triangle
 * 
 * @param x0 first x coordinate
 * 
 * @param y0 first y coordinate
 * 
 * @param x1 second x coordinate
 * 
 * @param y1 second y coordinate
 * 
 * @param x2 third x coordinate
 * 
 * @param y2 third x coordinate
*/
void GL::fillTriangleD(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    drawLineD(a, y0, b + 1, y0);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if (a > b)
      _swap_int16_t(a, b);
    if(y%2 == 0)
      drawLineD(a, y, b + 1, y);
    else
      drawLineDI(a, y, b + 1, y);
  }

  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b)
      _swap_int16_t(a, b);
    if(y%2 == 0)
      drawLineD(a, y, b + 1, y);
    else
      drawLineDI(a, y, b + 1, y);
  }
}

/**
 * @brief loads a tilemap for use in the graphics library
 * 
 * @param buffer buffer containing the tilemap
 * 
 * @param width tilemap width (in pixels), must be devisible by tile_w
 * 
 * @param height tilemap height (in pixels), must be devisible by tile_h
 * 
 * @param tile_w width of a tile (in pixels)
 * 
 * @param tile_h height of a tile (in pixels)
*/
bool GL::loadTileMap(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t tile_w, uint8_t tile_h)
{
  #ifndef UNSAFE_GL
  if((!width%tile_w == 0) || (!height%tile_h == 0))
    return false;
  #endif
  
  _texture_w = width;
  _texture_h = height;
  _tile_w = tile_w;
  _tile_h = tile_h;
  texture_buffer = buffer;
  return true;
}

/**
 * @brief loads a tile from tilemap into a texture binding
 * 
 * @param x the x position of the tile in map (coordinates increment per texture not pixel)
 * 
 * @param y the y position of the tile in map (coordinates increment per texture not pixel)
 * 
 * @param textureBinding the texture binding that the texture shouold be bound to (default is TEXTURE_BINDING_0)
*/
bool GL::loadTileFromMap(uint8_t x, uint8_t y, uint8_t textureBinding)
{
  #ifndef UNSAFE_GL
  if((x > (_texture_w/_tile_w-1)) || (y > (_texture_h/_tile_h-1)) || texture_buffer == NULL || !(textureBinding < MAX_TEX_BINDINGS))
    return false;
  #endif

  if(tex[textureBinding] != NULL)
    free(tex[textureBinding]);

  tex[textureBinding] = (uint8_t*)malloc((_tile_h*_tile_w)/8);
  w[textureBinding] = _tile_w;
  h[textureBinding] = _tile_h;
  uint8_t* texture = tex[textureBinding];
  uint16_t pointerPos1 = 0;
  uint16_t pointerPos2 = (y*((_texture_w/8)*(_tile_h/8)))+(x*(_tile_w/8));

  for(uint16_t i = 0; i < _tile_h; i++)
  {
    memcpy((void*)(((uint8_t*)texture)+pointerPos1), (void*)(((uint8_t*)texture_buffer)+pointerPos2), _tile_w/8);
    pointerPos1 += (_tile_w/8);
    pointerPos2 += (_texture_w/8);
  }

  return true;
}

/**
 * @brief loads a texture from buffer into texture binding
 * 
 * @param buffer buffer containing texture
 * 
 * @param width width of texture (in pixels)
 * 
 * @param height height of texture (in pixels)
*/
bool GL::loadTexture(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t textureBinding, bool dynamic)
{
  #ifndef UNSAFE_GL
  if(textureBinding < 0 || textureBinding > 2 || !(textureBinding < MAX_TEX_BINDINGS))
    return false;
  #endif

  w[textureBinding] = width;
  h[textureBinding] = height;
  tex[textureBinding] = buffer;
  dynamicTex[textureBinding] = dynamic;
  return true;
}

/**
 * @brief draw the texture at x and y coordinates to the display buffer (x coordinates are due to a tecnicality bound to multiples of 8) 
 * 
 * @param x coordinate for the texture to be draw to (on screen bound to multiples of 8)
 * 
 * @param y coordinate for the texture to be drawn to
 * 
 * @param textureBinding the binding from with to draw
*/
bool GL::drawTexture(uint16_t x, uint16_t y, uint8_t textureBinding)
{
  #ifndef UNSAFE_GL
  if(textureBinding < 0 || textureBinding > 7 || tex[textureBinding] == NULL)
    return false;
  if(!(x+w[textureBinding] < _w && x >= 0 && y+h[textureBinding] < _h && y >= 0))
    return false;
  #endif
  uint8_t* selected_texture = tex[textureBinding];
  uint16_t pointer_pos = (y*(_w/8))+((x-(x%8))/8);
  for(uint16_t i = 0; i < h[textureBinding]; i++)
  {
    memcpy((uint8_t*)(context_buffer+pointer_pos), (uint8_t*)(selected_texture+(i*w[textureBinding]/8)), (w[textureBinding]/8));
    pointer_pos+=(_w/8);
  }
  return true;
}

/**
 * @brief clear the texture stored at the buffer location
 * 
 * @param textureBinding texture binding to be cleared
*/
void GL::clearTexture(uint8_t textureBinding)
{
  if(dynamicTex[textureBinding])
  {
    free(tex[textureBinding]);
  }
  tex[textureBinding] = NULL;
  w[textureBinding] = 0;
  h[textureBinding] = 0;
  dynamicTex[textureBinding] = false;
}