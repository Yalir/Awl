/*
 *  Awl.cp
 *  Awl
 *
 *  Created by Ceylo on 12/08/11.
 *  Copyright 2011 Yalir. All rights reserved.
 *
 */

#include <iostream>
#include "Awl.h"
#include "AwlPriv.h"

void Awl::HelloWorld(const char * s)
{
	 AwlPriv *theObj = new AwlPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void AwlPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

