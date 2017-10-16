// SimpleChat.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "msgstruct.h"
#include <gui.hpp>
#include <gui/widgets/label.hpp>
#include <gui/widgets/button.hpp>
#include "mregister.h"
int main()
{
	using namespace nana;
	form fm(API::make_center(400,300),appearance(true,true,false,false,true,false,false));
	button but_signin(fm,rectangle(0,0,80,30));
	but_signin.caption("Sign In");
	fm.caption("Simple Chat");
	label lb{ fm, rectangle{ 10, 50, 150, 20 } };
	lb.caption("<b>Hello, world!</b>");
	fm.show();
	mregister test;
	exec();
}