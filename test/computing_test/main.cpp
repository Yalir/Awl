//
//  main.cpp
//  awl_test
//
//  Created by Ceylo on 12/08/11.
//  Copyright 2011 Yalir. All rights reserved.
//

#include <Awl/Awl.hpp>
#include <Awl/Debug.hpp>

//class test {
//public:
//	void tt(void)
//	{
//		std::cout << "method!" << std::endl;
//	}
//	
//};
//
//void some_func(void)
//{
//	DISPLAY_THREAD_ID;
//	std::cout << "function!" << std::endl;
//	
//	AwlMainThreadBlock
//	{
//		DISPLAY_THREAD_ID;
//		std::cout << "o_o" << std::endl;
//		aw::WorkLoop::Default().Stop();
//	}
//	AwlCloseMainThreadBlock;
//}

int main (int argc, const char * argv[])
{	
	for (int i = 0; i < 50;i++)
	{
		// Do some parallel computing
		AwlAsyncBlock
		{
			int a = 1;
			for (int i = 0; i < 1000000;i++)
				for (int j = 0; j < 1000;j++)
					a = a * (a + 1);
		}
		AwlCloseAsyncBlock;
	}
	//DISPLAY_THREAD_ID;
	
	//AwlAsyncCall(some_func);
//	test t;
//	AwlAsyncMethod(&test::tt, &t);
	
	while (aw::WorkLoop::Default().Run());
	
    return 0;
}

