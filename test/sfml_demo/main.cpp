
/*
 *  sfml_demo/main.cpp
 *  Awl - Asynchronous Work Library
 *
 *  Copyright (c) 2011 Lucas Soltic
 *  ceylow@gmail.com
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it freely,
 *  subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *     you must not claim that you wrote the original software.
 *     If you use this software in a product, an acknowledgment
 *     in the product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *     and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 *
 */

#include <Awl/Awl.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

sf::Texture tex1;
sf::Texture tex2;
sf::Texture tex3;
bool loaded[3] = {false};

int main()
{
	sf::RenderWindow win(sf::VideoMode(640, 480), "Image Loader");
	win.SetFramerateLimit(60);
	
	// Create textures and sprites
	tex1.Create(1000, 700);
	tex2.Create(1000, 700);
	tex3.Create(1000, 700);
	
	sf::Sprite sp1(tex1);
	sf::Sprite sp2(tex2);
	sf::Sprite sp3(tex3);
	
	sp1.SetPosition(10, 10);
	sp2.SetPosition(50, 50);
	sp3.SetPosition(100, 100);
	
	// Execute this block in an asynchronous way
	AwlAsyncBlock
	({
		tex1.LoadFromFile("big_image1.png");
		glFlush(); // Make sure the texture is updated in the main thread's GL context
		
		// We don't want to care about concurrent access issues, so execute this
		// on the main thread ;
		// We could also protect our actions with a mutex, that's your choice
		AwlMainThreadBlock
		({
			// Suppose this is a non-atomic operation and suppose that doing
			// this in a concurrent way would cause issues
			loaded[0] = true;
		})
	})
	
	// Repeat with second texture
	AwlAsyncBlock
	({
		tex2.LoadFromFile("big_image2.png");
		glFlush();
		
		AwlMainThreadBlock ({
			loaded[1] = true;
		})
	})
	
	// Repeat with third texture
	AwlAsyncBlock
	({
		tex3.LoadFromFile("big_image3.png");
		glFlush();
		
		AwlMainThreadBlock ({
			loaded[2] = true;
		})
	})
	
	while (win.IsOpened() && awl::WorkLoop::Default().Run())
	{
		sf::Event ev;
		// Here we run the WorkLoop to process the tasks we wanted to perform
		// on the main thread
		while (win.PollEvent(ev))
		{
			if (ev.Type == sf::Event::Closed)
				win.Close();
			
			if (ev.Type == sf::Event::KeyPressed && ev.Key.Code == sf::Keyboard::Escape)
				win.Close();
		}
		
		win.Clear();
		
		// Show how loading progresses (we may not see any difference with 3 images but.. whatever)
		if (loaded[0]) win.Draw(sp1);
		if (loaded[1]) win.Draw(sp2);
		if (loaded[2]) win.Draw(sp3);
		
		win.Display();
	}
}
