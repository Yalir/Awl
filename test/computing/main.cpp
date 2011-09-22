
/*
 *  computing/main.cpp
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
#include <Awl/Debug.hpp>

int counter = 0;

class TestClass {
public:
	void method(awl::Task *self)
	{
		MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
	}
	
	static void static_method(awl::Task *self)
	{
		MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
		
		AwlAsyncBlock
		({
			MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
		})
	}
};

void function(awl::Task *self)
{
	MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
	
	self->Abort();
	
	// Should never be executed
	AwlAsyncMainThreadBlock
	({
		MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
	})
}

int main (int argc, const char * argv[])
{	
	for (int i = 0; i < 3;i++)
	{
		// Do some parallel computing
		AwlAsyncBlock
		({
			int a = 1;
			for (int i = 0; i < 1000000;i++)
				for (int j = 0; j < 10;j++)
					a = a * (a + 1);
			
			MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
		})
	}
	
	TestClass obj;
	
	AwlAsyncCall(function);
	AwlAsyncCall(TestClass::static_method);
	AwlAsyncMethod(&TestClass::method, &obj);
	
	awl::TaskRef task;
	AwlAsyncManagedBlock
	(task,
	{
		MT_COUT(std::cout << "check point " << __LINE__ << " ok:" << ++counter << std::endl);
	})
	task->Wait();
	
	awl::ThreadPool::WaitAndDie();
	awl::WorkLoop::Default().Run(); // call here to be sure function() has been executed
	
    return 0;
}

