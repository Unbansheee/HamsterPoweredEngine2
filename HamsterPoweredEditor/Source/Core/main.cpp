
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


#include "App.h"
#include "Window.h"


int main()
{
	
	App::Instance().Begin();

	//Update loop
	while (App::Instance().window->IsOpen())
	{
		App::Instance().Update();
	}

	delete App::Instance().window;
	return 0;
}

