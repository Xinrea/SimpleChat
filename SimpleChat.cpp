// SimpleChat.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include <gui.hpp>
#include <gui/widgets/label.hpp>
#include <gui/widgets/button.hpp>
#include <gui/widgets/textbox.hpp>
#include <gui/msgbox.hpp>
#include <threads/pool.hpp>
#include "mregister.h"
#include "mlogin.h"
unsigned session;

class mainWindow: public nana::form
{
private:
	nana::button but_signin;
	nana::label lable_account, lable_password;
	nana::textbox tb_account, tb_password;
	nana::threads::pool pool_;
public:
	mainWindow()
	{
		this->caption(L"登录");
		nana::place plc(*this);
		but_signin.create(*this); but_signin.caption(L"登录");
		but_signin.events().click(nana::threads::pool_push(pool_, *this,&mainWindow::login));
		lable_account.create(*this); lable_account.caption(L"账号:");
		lable_password.create(*this); lable_password.caption(L"密码:");
		tb_account.create(*this); tb_password.create(*this);
		plc.div("vert<vert<><<weight=20%><l1><t1 weight=50%><weight=20%>weight=10%><weight=5%><<weight=20%><l2><t2 weight=50%><weight=20%>weight=10%><>  weight=70%><<weight=30%><bsignin><weight=30%>><>");
		plc["l1"] << lable_account;
		plc["l2"] << lable_password;
		plc["t1"] << tb_account;
		plc["t2"] << tb_password;
		plc["bsignin"] << but_signin;
		plc.collocate();
		this->caption("Simple Chat");
	}
private:
	void login()
	{
		std::string pwd;
		char pwd_cstr[12];
		mlogin login_t;
		but_signin.caption(L"连接服务器中");
		but_signin.enabled(false);
		tb_password.getline(12, pwd);
		strcpy_s(pwd_cstr, 12, pwd.c_str());
		if (!login_t.login(tb_account.to_int(), pwd_cstr))
		{
			nana::msgbox messagebox("ERROR");
			messagebox << "Login Failed";
			messagebox.show();
		}
		else session = login_t.getSession();
		but_signin.caption(L"登录");
		but_signin.enabled(true);
	}
};


int main()
{
	mainWindow window;
	window.show();
	nana::exec();
	return 0;
}