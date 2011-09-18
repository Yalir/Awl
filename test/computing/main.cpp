//
//  main.cpp
//  awl_test
//
//  Created by Ceylo on 12/08/11.
//  Copyright 2011 Yalir. All rights reserved.
//

#include <Awl/Awl.hpp>
#include <Awl/Debug.hpp>

class TestClass {
public:
	void test(void)
	{
		std::cout << "check point 1 ok" << std::endl;
	}
	
};

void some_func(void)
{
	DISPLAY_THREAD_ID;
	std::cout << "function!" << std::endl;
	
	AwlMainThreadBlock
	({
		DISPLAY_THREAD_ID;
		std::cout << "o_o" << std::endl;
		awl::WorkLoop::Default().Stop();
	})
}

int main (int argc, const char * argv[])
{	
	for (int i = 0; i < 50;i++)
	{
		// Do some parallel computing
		AwlAsyncBlock
		({
			int a = 1;
			for (int i = 0; i < 1000000;i++)
				for (int j = 0; j < 100;j++)
					a = a * (a + 1);
		})
	}
	
	
	awl::ThreadPool::WaitAndDie();
	
    return 0;
}

