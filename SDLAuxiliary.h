#pragma once

class SDLAuxiliary
{
public:
	// Draw a curled line between two points on the screen renderer with a given amplitude and a number of half waves.
	static int curlRGBA(SDL_Renderer * renderer, const Sint16 x1, const Sint16 y1, const Sint16 x2, const Sint16 y2,
             const Sint16 amp,const Sint16 num_half_waves, int steps, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	// Draw a pixel with variable width
	static void drawPixel(SDL_Renderer *renderer, const int x, const int y, const int width, const int scale, const int red = 0);
	
	//Log an SDL error with some error message to the output stream of our choice
	static void logSDLError(std::ostream &os, const std::string &msg);

	//Log an SDL debug output with some message to the output stream of our choice
	static void logSDLDebug(std::ostream &os, const std::string &msg);

	static void logSDLDebug(std::ostream &os, const int msg);
};