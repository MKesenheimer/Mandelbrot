#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>

#include "SDLAuxiliary.h"
#include "Main.h"
#include "Timer.h"
#include "Functions.h"
#include "Transformer.h"
#include "SetCalculator.h"

constexpr int SCREEN_WIDTH  = 900;
constexpr int SCREEN_HEIGHT = 900;
constexpr int FRAMES_PER_SECOND = 20; //Fps auf 20 festlegen

int main( int argc, char* args[]) {

	int frame = 0; //take records of frame number
	bool cap = true; //Framecap an oder ausschalten
    
    //Timer zum festlegen der FPS
	Timer fps;
    //Timer zum errechnen der weltweit vergangenen Zeit
	Timer worldtime;
    worldtime.start();
    
    //calculate the small time between two frames in ms
    int oldTime = 0;
    int newTime = 0;
    int dt = 1;
    
	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDLAuxiliary::logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	//Setup our window and renderer, this time let's put our window in the center
	//of the screen
	SDL_Window *window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDLAuxiliary::logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDLAuxiliary::logSDLError(std::cout, "CreateRenderer");
		SDLAuxiliary::cleanup(window);
		SDL_Quit();
		return 1;
	}
	
	// flags
	bool quit = false;
	bool update = true;
	bool updateMouse = false;
	bool updateConstant = false;
	bool highRes = false;
	int wheel = 0;
	int MOUSE_X = SCREEN_WIDTH / 2, MOUSE_Y = SCREEN_HEIGHT / 2;
	double X0 = 0.0; //-0.74;
	double Y0 = 0.0; //0.21;
	std::complex<double> constant(0, 0);
	
	//our event structure
	SDL_Event e;
	while (!quit)
	{
        //start the fps timer
        fps.start();
        
		//Read any events that occured, for now we'll just quit if any event occurs
		while (SDL_PollEvent(&e))
		{
			//If user closes the window
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//If user presses any key
			if (e.type == SDL_KEYDOWN)
			{
                //do something
				if (e.key.keysym.sym == SDLK_c)
				{
                    cap = !cap;
				}
				
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					update = true;
					highRes = !highRes;
					if (highRes)
						std::cout << "high resolution enabled." << std::endl;
					else
						std::cout << "high resolution disabled." << std::endl;
				}
				
			}
			if (!highRes)
			{
				//SDL_PumpEvents();
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);
					update = true;
					updateMouse = true;
					//SDL_Log("Mouse Button 2 (right) is pressed.");
				}
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);
					update = true;
					updateConstant = true;
					//SDL_Log("Mouse Button 2 (right) is pressed.");
				}
				if (e.type == SDL_MOUSEWHEEL)
				{
					//SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);
					update = true;
					//std::cout << wheel << std::endl;
					if (e.wheel.y > 0)
						wheel++;
					else
						wheel--;
					
					if (wheel < 0)
						wheel = 0;
					//SDL_Log("Mouse Wheel event");
				}
			}
		}
		
		if (update)
		{
			//Rendering
			SDL_RenderClear(renderer);
			//Draw the background white
			boxRGBA(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 255);
		
			// number of Pixels per SCREEN_WIDTH
			double resolution = 200;
			if (highRes)
				resolution = 1000;
			if (resolution >= SCREEN_WIDTH)
				resolution = SCREEN_WIDTH;
		
			double zoom = 5;
			if (wheel > 0)
				zoom /= std::pow(2.0, wheel);
			//const double angle = 0.0;
	
			if (updateMouse)
			{
				double mouseX = static_cast<double>(MOUSE_X);
				double mouseY = static_cast<double>(MOUSE_Y);
				// apply transformations to the mouse coordinates
				// (see lines 262 and following for better explanation)
				Transformer::Linear(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, 1.0 / SCREEN_WIDTH, -1.0 / SCREEN_WIDTH, mouseX, mouseY);
				Transformer::Linear(-X0, -Y0, 1.0, 1.0, mouseX, mouseY);
				Transformer::Linear(X0, Y0, zoom, zoom, mouseX, mouseY);
				Transformer::Linear(-X0, -Y0, 1.0, 1.0, mouseX, mouseY);
				X0 = mouseX;
				Y0 = mouseY;
				std::cout << "mouse coord. = " << mouseX << ", " << mouseY << ", zoom = " << zoom << std::endl;
			}
		
			if (updateConstant)
			{
				double mouseX = static_cast<double>(MOUSE_X);
				double mouseY = static_cast<double>(MOUSE_Y);
				// apply transformations to the mouse coordinates
				Transformer::Linear(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, 4.0 / SCREEN_WIDTH, -4.0 / SCREEN_WIDTH, mouseX, mouseY);
				constant = std::complex<double>(mouseX, mouseY);
				std::cout << "constant " << constant << std::endl;
			}
			
			const double deltaX = SCREEN_WIDTH / resolution;
			const double deltaY = deltaX;
			for (int i = 0; i <= resolution; ++i)
			{
				for (int j = 0; j <= resolution; ++j)
				{
					double X = i * deltaX, Y = j * deltaY;
					
					// transform the coordinates of the screen into a coordinate
					// system where the origin is in the middle of the screen and
					// where the scale is (-0.5, 0.5) in both directions
					// window -> coordinate system
					// (0, 0) -> (-0.5, 0.5)
					// (SCREEN_WIDTH, 0) -> (0.5, 0.5)
					// (0, SCREEN_HEIGHT) -> (-0.5, -0.5)
					// (SCREEN_WIDTH, SCREEN_HEIGHT) -> (0.5, -0.5)
					Transformer::Linear(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, 1.0 / SCREEN_WIDTH, -1.0 / SCREEN_WIDTH, X, Y);
					
					// apply the additional scalings and transformations
					Transformer::Linear(-X0, -Y0, 1.0, 1.0, X, Y); // move to new origin
					Transformer::Linear(X0, Y0, zoom, zoom, X, Y); // scale everything around the new origin (this moves the origin)
					Transformer::Linear(-X0, -Y0, 1.0, 1.0, X, Y); // go back to origin
					
					const std::complex<double> point(X, Y);
					//int divstr = 30 * SetCalculator::Mandelbrot(point, constant, 100);
					int divstr = 30 * SetCalculator::Julia(point, constant, 100);
					
					//std::cout << point << std::endl;
					
					// mark the new origin
					int isOrigin = 0;
//#define MARKPOINT
#ifdef MARKPOINT
					if (std::abs(X - X0) <= 0.01*zoom && std::abs(Y - Y0) <= 0.01*zoom)
					{
						divstr = 255;
						isOrigin = 1;
					}
#endif
					
					// undo the additional transformations
					Transformer::Linear(X0, Y0, 1.0, 1.0, X, Y);
					Transformer::Linear(-X0 * zoom, -Y0 * zoom, 1.0 / zoom, 1.0 / zoom, X, Y);
					Transformer::Linear(X0, Y0, 1.0, 1.0, X, Y);
					
					// transform back
					Transformer::Linear(-0.5, 0.5, SCREEN_WIDTH, -SCREEN_WIDTH, X, Y);
					
					//std::cout << X << ", " << Y << std::endl << std::endl;
					
					SDLAuxiliary::drawPixel(renderer,
						static_cast<int>(X),
						static_cast<int>(Y),
						static_cast<int>(deltaX), divstr, isOrigin);
				}
			}
			
			// render
			SDL_RenderPresent(renderer);

			update = false;
			updateMouse = false;
			updateConstant = false;
		}
        
        // Timer related stuff
        oldTime = newTime;
        newTime = worldtime.getTicks();
        if (newTime > oldTime)
		{
            dt = (newTime - oldTime) / 1000.; // small time between two frames in s
        }
        if(dt == 0) dt = 1;
        
        //increment the frame number
        frame++;
        //apply the fps cap
		if((cap == true) && (fps.getTicks() < 1000/FRAMES_PER_SECOND))
		{
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.getTicks() );
		}
            
        //update the window caption
		if(worldtime.getTicks() > 1000)
		{
			std::stringstream caption;
			caption << "Mandelbrot, FPS = " << 1000.f*frame/worldtime.getTicks();
            SDL_SetWindowTitle(window,caption.str().c_str());
			worldtime.start();
            frame = 0;
		}
	}
    
	//Destroy the various items
	SDLAuxiliary::cleanup(renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}