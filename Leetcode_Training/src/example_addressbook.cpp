#include <string>
#include <iostream>

//快捷注释 ctrl + k + c； 取消注释 ctrl + k + u

#define MAX 1000 //通讯录最多有1000个人

struct Person
{
	std::string m_Name;
	int m_Age;
	int m_Gender;
	std::string m_Address;
};

Person Persons[MAX];
Person* PersonPrt = Persons;
int NumofPerson = 0;

void ShowMenu()
{
	std::cout << "*****************************" << std::endl
		<< "*****\t1、添加联系人\t*****" << std::endl
		<< "*****\t2、显示联系人\t*****" << std::endl
		<< "*****\t3、删除联系人\t*****" << std::endl
		<< "*****\t4、查找联系人\t*****" << std::endl
		<< "*****\t5、修改联系人\t*****" << std::endl
		<< "*****\t6、清空联系人\t*****" << std::endl
		<< "*****\t0、退出通讯录\t*****" << std::endl;
}
void clear()
{
	std::cin.get();	std::cin.get();
	std::cout << "\033c";
	//也可以#include<stdlib.h>，用 system("pause") + system("cls")来完成，效果更好
}

void AddPerson()
{
	if (NumofPerson == MAX)
		std::cout << "通讯录已满" << std::endl;
	else
	{
		std::cout << "请输入姓名" << std::endl;
		std::cin >> PersonPrt->m_Name;
		std::cout << "请输入年龄" << std::endl;
		std::cin >> PersonPrt->m_Age;
		std::cout << "请输入性别" << std::endl << "\t 1 -- 男" << std::endl << "\t 2 -- 女" << std::endl;
		int Gender = 0;
		while (true)
		{
			std::cin >> Gender;
			if (Gender == 1 || Gender == 2)
			{
				PersonPrt->m_Gender = Gender;
				break;
			}
			std::cout << "性别输入有误" << std::endl;
		}
		std::cout << "请输入地址" << std::endl;
		std::cin >> PersonPrt->m_Address;
		std::cout << "录入成功" << std::endl;
		PersonPrt++; NumofPerson++;
		clear();
	}
}
void AddPerson(Person* Prt)
{
	std::cout << "请输入姓名" << std::endl;
	std::cin >> Prt->m_Name;
	std::cout << "请输入年龄" << std::endl;
	std::cin >> Prt->m_Age;
	std::cout << "请输入性别" << std::endl << "\t 1 -- 男" << std::endl << "\t 2 -- 女" << std::endl;
	int Gender = 0;
	while (true)
	{
		std::cin >> Gender;
		if (Gender == 1 || Gender == 2)
		{
			Prt->m_Gender = Gender;
			break;
		}
		std::cout << "性别输入有误" << std::endl;
	}
	std::cout << "请输入地址" << std::endl;
	std::cin >> Prt->m_Address;
	std::cout << "录入成功" << std::endl;
	clear();
}

void ShowAll()
{
	Person* StartPrt = Persons;
	if (NumofPerson == 0)
	{
		std::cout << "通讯录为空" << std::endl;
	}
	else
	{
		while (StartPrt != PersonPrt)
		{
			std::cout << "姓名：" << StartPrt->m_Name << "\t";
			std::cout << "年龄：" << StartPrt->m_Age << "\t";
			std::cout << "性别：" << (StartPrt->m_Gender == 1 ? "男" : "女") << "\t";
			std::cout << "地址：" << StartPrt->m_Address << "\t" << std::endl;
			StartPrt++;
		}
	}
	clear();
}
void ShowOne(Person* Prt)
{
	std::cout << "姓名：" << Prt->m_Name << "\t";
	std::cout << "年龄：" << Prt->m_Age << "\t";
	std::cout << "性别：" << (Prt->m_Gender == 1 ? "男" : "女") << "\t";
	std::cout << "地址：" << Prt->m_Address << "\t";
}

void DeleteAll()
{
	PersonPrt = Persons;
	NumofPerson = 0;
	std::cout << "通讯录已清空" << std::endl;
	clear();
}
void DeleteOne(Person* Prt)
{
	while (Prt != (PersonPrt - 1))
	{
		Prt->m_Name = (Prt + 1)->m_Name;
		Prt->m_Age = (Prt + 1)->m_Age;
		Prt->m_Gender = (Prt + 1)->m_Gender;
		Prt->m_Address = (Prt + 1)->m_Address;
		Prt++;
	}
	PersonPrt--; NumofPerson--;
	std::cout << "所选对象已删除" << std::endl;
}

//未解决重名问题，对重名对象，优先检索出序号靠前的
Person* PersonCheck()
{
	Person* StartPrt = Persons;
	std::string nameforcheck;
	if (NumofPerson == 0)
	{
		std::cout << "通讯录为空" << std::endl;
		return NULL;
	}
	else
	{
		std::cout << "请输入待查找姓名" << std::endl;
		std::cin >> nameforcheck;
		while (StartPrt != PersonPrt)
		{
			if (nameforcheck == StartPrt->m_Name)
			{
				return StartPrt;
			}
			StartPrt++;
		}
		std::cout << "通讯录中无此人";
		return NULL;
	}
	clear();
}

int main()
{
	bool MenuFlag = 1;
	int FunctionSwitch;

	while (MenuFlag)
	{
		ShowMenu();
		std::cin >> FunctionSwitch;
		switch (FunctionSwitch)
		{
		case 1://添加联系人
			AddPerson();
			break;
		case 2://显示联系人
			ShowAll();
			break;
		case 3:	//根据姓名删除联系人
		{
			Person* PrtDle = PersonCheck();
			if (PrtDle != NULL)
				DeleteOne(PrtDle);
			clear();
		}
		break;
		case 4: //根据姓名查找联系人
		{
			Person* PrtChk = PersonCheck();
			if (PrtChk != NULL)
				ShowOne(PrtChk);
			clear();
		}
		break;
		case 5: //根据姓名修改联系人信息
		{
			Person* PrtRe = PersonCheck();
			AddPerson(PrtRe);
		}
		break;
		case 6: //清空通讯录
			DeleteAll();
			break;
		case 0: //退出
		{
			MenuFlag = 0;
			std::cout << "感谢使用" << std::endl;
			clear();
		}
		break;
		default:
			std::cout << "Error" << std::endl;
			clear();
		}
	}
}