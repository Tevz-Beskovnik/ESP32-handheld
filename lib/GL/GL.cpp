#include <GL.h>

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
: Adafruit_SharpMem(clk, mosi, cs, width, height, freq), texture_buffer(NULL), _w(width), _h(height)
{
  tex[0] = NULL;
  tex[1] = NULL;
  tex[2] = NULL;
}

GL::~GL()
{
  if(tex[0] != NULL)
    free(tex[0]);
  if(tex[1] != NULL)
    free(tex[1]);
  if(tex[2] != NULL)
    free(tex[2]);
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
  Serial.printf("%i\n", width*height/8);
  texture_buffer = (uint8_t*)malloc((width*height)/8);
  memcpy((void*)texture_buffer, (void*)buffer, (width*height)/8);
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
bool GL::loadTexture(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t textureBinding)
{
  #ifndef UNSAFE_GL
  if(textureBinding < 0 || textureBinding > 2 || !(textureBinding < MAX_TEX_BINDINGS))
    return false;
  #endif

  w[textureBinding] = width;
  h[textureBinding] = height;
  tex[textureBinding] = (uint8_t*)malloc((width*height)/8);
  memcpy(tex[textureBinding], buffer, (width*height)/8);
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
