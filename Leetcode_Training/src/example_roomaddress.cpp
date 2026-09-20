#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

// 最开始读取默认为utf-8编码的txt文件出现乱码，于是将visual studio改成utf-8,
// 但是又因为代码中的中文按照utf-8会出错，导致无法通过编译 error C2001
// 如果强制在命令行中加入/utf-8，cmd显示时会乱码
// 于是在命令行加入以下参数
// /source-charset:utf-8 /execution-charset:gbk 
// 按照utf-8编码读取，但是字符串按照gbk编码保存，所以此时代码中的中文在按照gbk编码的cmd中可以正常显示
// 但是如果在代码中用ifstream读取utf-8编码的文本文件，还是会乱码
// 要将文本文件手动设置成gbk格式

#define ADMIN_FILE "../file/admin.txt"
#define STUDENT_FILE "../file/student.txt"
#define TEACHER_FILE "../file/teacher.txt"
#define COMPUTER_FILE "../file/computer.txt"
#define ORDER_FILE "../file/order.txt"

map<int, string> Workdays = { {1, "周一"}, {2, "周二"}, {3, "周三"}, {4, "周四"}, {5, "周五"} };
map<int, string> Intervals = { {1, "上午"}, {2, "下午"} };
map<int, string> orderStatus = { {1, "审核中"}, {2, "预约成功"}, {3, "预约失败"}, {4, "已取消"} };

struct Computer
{
	int mID;
	int mCap;
};

class Order
{
public:
	int date;
	int interval;
	int mID;
	string mName;
	int room;
	int status;

	Order() {};
	Order(int d, int i, int id, string name, int r, int s)
		: date(d), interval(i), mID(id), mName(name), room(r), status(s) {}

};

class Identity
{
public:
	vector<Computer> vCom;
	vector<Order> vOrd;

	Identity() {}
	Identity(string name, string pwd): mName(name), mPwd(pwd)
	{
		ifstream ifs;
		ifs.open(COMPUTER_FILE, ios::in);
		Computer c;
		while (ifs >> c.mID && ifs >> c.mCap)
			vCom.push_back(c);
		ifs.close();

		ifs.open(ORDER_FILE, ios::in);
		Order o;
		while (ifs >> o.date && ifs >> o.interval && ifs >> o.mID && ifs >> o.mName && ifs >> o.room && ifs >> o.status)
			vOrd.push_back(o);
		ifs.close();
	}
	// 子菜单页面
	virtual void openMenu() = 0;
	void updateOrder()
	{
		if (vOrd.size() == 0)
			return;
		ofstream ofs(ORDER_FILE, ios::trunc);
		for (auto it : vOrd)
		{
			ofs << it.date << " ";
			ofs << it.interval << " ";
			ofs << it.mID << " ";
			ofs << it.mName << " ";
			ofs << it.room << " ";
			ofs << it.status << endl;
		}
		ofs.close();
	}

	string mName;
	string mPwd;
};

class Student :public Identity
{
public:
	int mID;
	int hasOrder;
	
	Student() {}
	Student(int id, string name, string pwd) : Identity(name, pwd), mID(id), hasOrder(0) { this->checkHasOrder(); }

	virtual void openMenu() 
	{
		cout << "欢迎学生：" << this->mName << "登陆！" << endl;
		cout << "\t\t -----------------\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t|    1.申请预约   |\n";
		cout << "\t\t|    2.我的预约   |\n";
		cout << "\t\t|    3.查看预约   |\n";
		cout << "\t\t|    4.取消预约   |\n";
		cout << "\t\t|    0.注销登录   |\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t -----------------\n";
		cout << "请输入您的选择：";
	}

	void applyOrder() 
	{
		if (hasOrder == 1)
		{
			cout << "已有申请中的预约" << endl;
			system("pause");
			system("cls");
			return;
		}
		cout << "机房开放时间：周一至周五" << endl;
		cout << "1.周一" << endl;
		cout << "2.周二" << endl;
		cout << "3.周三" << endl;
		cout << "4.周四" << endl;
		cout << "5.周五" << endl;
		cout << "请输入预约时间" << endl;
		int date = 0;
		int interval = 0;
		int room = 0;

		while (true)
		{
			cin >> date;
			if (date >= 1 && date <= 5)
				break;
			cout << "输入有误，重新输入" << endl;
		}

		cout << "请输入预约时间段" << endl;
		cout << "1.上午" << endl;
		cout << "2.下午" << endl;
		while (true)
		{
			cin >> interval;
			if (interval >= 1 && interval <= 2)
				break;
			cout << "输入有误，重新输入" << endl;
		}

		cout << "请选择机房：" << endl;
		cout << "1号机房容量：" << vCom[0].mCap << endl;
		cout << "2号机房容量：" << vCom[1].mCap << endl;
		cout << "3号机房容量：" << vCom[2].mCap << endl;

		while (true)
		{
			cin >> room;
			if (room >= 1 && room <= 3)
				break;
			cout << "输入有误，重新输入" << endl;
		}
		cout << "预约成功，审核中" << endl;
		hasOrder = 1;
		vOrd.push_back(Order(date, interval, mID, mName, room, 1));
		ofstream ofs(ORDER_FILE, ios::app);
		ofs << date << " ";
		ofs << interval << " ";
		ofs << mID << " ";
		ofs << mName << " ";
		ofs << room << " ";
		ofs << 1 << endl;
		ofs.close();

		system("pause");
		system("cls");
	}

	void showMyOrder() 
	{
		if (vOrd.size() == 0)
		{
			cout << "目前无预约信息" << endl;
			system("pause");
			system("cls");
			return;
		}
		int orderNum = 0;
		for (auto it : vOrd)
		{
			if (mID == it.mID)
			{
				++orderNum;
				cout << "星期: " << Workdays[it.date] << " ";
				cout << "时段：" << Intervals[it.interval] << " ";
				cout << "学号：" << mID << " ";
				cout << "姓名：" << mName << " ";
				cout << "机房：" << it.room << "号机房 ";
				cout << "预约状态：" << orderStatus[it.status] << endl;
			}
		}
		if (orderNum == 0)
			cout << "没有您的预约信息" << endl;
		system("pause");
		system("cls");
	}

	void showAllOrder() 
	{
		if (vOrd.size() == 0)
		{
			cout << "目前无预约信息" << endl;
			system("pause");
			system("cls");
			return;
		}
		for (auto it : vOrd)
		{
			cout << "星期: " << Workdays[it.date] << " ";
			cout << "时段：" << Intervals[it.interval] << " ";
			cout << "学号：" << it.mID << " ";
			cout << "姓名：" << it.mName << " ";
			cout << "机房：" << it.room << "号机房 ";
			cout << "预约状态：" << orderStatus[it.status] << endl;
		}
		system("pause");
		system("cls");
	}

	void cancelOrder() 
	{
		vector<Order*> orders;
		int orderNum = 0;
		for (auto& it : vOrd)
		{
			int index = 0;
			if (mID == it.mID && it.status != 4)
			{
				++orderNum;
				orders.push_back(&it);
				cout << ++index << ". ";
				cout << "星期: " << Workdays[it.date] << " ";
				cout << "时段：" << Intervals[it.interval] << " ";
				cout << "学号：" << mID << " ";
				cout << "姓名：" << mName << " ";
				cout << "机房：" << it.room << "号机房 ";
				cout << "预约状态：" << orderStatus[it.status] << endl;
			}
		}
		if (orderNum > 0)
		{
			int index = 0;
			cout << "请输入要取消的预约的序号" << endl;
			cin >> index;
			orders[index - 1]->status = 4;
			updateOrder();
		}
		else
		{
			cout << "没有您的预约信息" << endl;
			return;
		}
		cout << "预约已取消" << endl;
		system("pause");
		system("cls");
	}

	void checkHasOrder()
	{
		for (auto& it : vOrd)
		{
			if (mID == it.mID && it.status != 4)
			{
				hasOrder = 1;
			}
		}
	}


};

class Teacher :public Identity
{
public:
	int mID;

	Teacher() {}
	Teacher(int id, string name, string pwd): Identity(name, pwd), mID(id) {}

	virtual void openMenu() 
	{
		cout << "欢迎教师：" << this->mName << "登陆！" << endl;
		cout << "\t\t -----------------\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t|    1.查看预约   |\n";
		cout << "\t\t|    2.审核预约   |\n";
		cout << "\t\t|    0.注销登录   |\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t -----------------\n";
		cout << "请输入您的选择：";
	}

	void showAllOrder() 
	{
		if (vOrd.size() == 0)
		{
			cout << "目前无预约信息" << endl;
			system("pause");
			system("cls");
			return;
		}
		for (auto it : vOrd)
		{
			cout << "星期: " << Workdays[it.date] << " ";
			cout << "时段：" << Intervals[it.interval] << " ";
			cout << "学号：" << it.mID << " ";
			cout << "姓名：" << it.mName << " ";
			cout << "机房：" << it.room << "号机房 ";
			cout << "预约状态：" << orderStatus[it.status] << endl;
		}
		system("pause");
		system("cls");
	}

	
	void validOrder() 
	{
		vector<Order*> orders;
		int orderNum = 0;
		for (auto& it : vOrd)
		{
			int index = 0;
			if (it.status == 1)
			{
				++orderNum;
				orders.push_back(&it);
				cout << ++index << ". ";
				cout << "星期: " << Workdays[it.date] << " ";
				cout << "时段：" << Intervals[it.interval] << " ";
				cout << "学号：" << it.mID << " ";
				cout << "姓名：" << it.mName << " ";
				cout << "机房：" << it.room << "号机房 ";
				cout << "预约状态：" << orderStatus[it.status] << endl;
			}
		}
		if (orderNum > 0)
		{
			int index = 0, select = 0;
			cout << "请输入要审核的预约的序号" << endl;
			cin >> index;
			cout << "请输入审核操作：" << endl;
			cout << "1.通过" << endl;
			cout << "2.不通过" << endl;
			cout << "0.返回" << endl;
			cin >> select;
			if (select == 1)
			{
				orders[index - 1]->status = 2;
				updateOrder();
			}
			else if (select == 2)
			{
				orders[index - 1]->status = 3;
				updateOrder();
			}
			else if (select == 0)
			{
				cout << "审核完毕" << endl;
				system("pause");
				system("cls");
				return;
			}
		}
		else
		{
			cout << "目前无需要审核的信息" << endl;
			system("pause");
			system("cls");
			return;
		}
		cout << "审核完毕" << endl;
		system("pause");
		system("cls");
	}

};

class Admin :public Identity
{
public:
	vector<Student> vStu;
	vector<Teacher> vTea;

	Admin() {}
	Admin(string name, string pwd) : Identity(name, pwd) 
	{ 
		this->intializeVec(); 
		cout << "当前机房数量为：" << vCom.size() << endl;
	}

	virtual void openMenu() 
	{
		cout << "欢迎管理员：" << this->mName << "登陆！" << endl;
		cout << "\t\t -----------------\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t|    1.添加账号   |\n";
		cout << "\t\t|    2.查看账号   |\n";
		cout << "\t\t|    3.查看机房   |\n";
		cout << "\t\t|    4.清空预约   |\n";
		cout << "\t\t|    0.注销登录   |\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t -----------------\n";
		cout << "请输入您的选择：";
	}

	void addAccount() 
	{
		cout << "请输入添加账号的类型" << endl;
		cout << "1.添加学生" << endl;
		cout << "2.添加教师" << endl;
		string fileName;
		int id;
		string name;
		string pwd;

		int select = 0;
		string errorTip;
		while (true) 
		{
			cin >> select;
			if (select == 1)
			{
				fileName = STUDENT_FILE;
				cout << "请输入学号：";
				errorTip = "输入学号重复";
				break;
			}
			else if (select == 2)
			{
				fileName = TEACHER_FILE;
				cout << "请输入职工号：";
				errorTip = "输入职工号重复";
				break;
			}
			else
			{
				cout << "请输入正确账号类型" << endl;
			}
		}
		ofstream ofs;
		ofs.open(fileName, ios::out | ios::app);// append

		while (true)
		{
			cin >> id;
			if (checkRepeat(id, select))
			{
				cout << errorTip << endl;
			}
			else
			{
				break;
			}
		}
		cout << "请输入姓名" << endl;
		cin >> name;
		cout << "请输入密码" << endl;
		cin >> pwd;
		if (select == 1)
			vStu.push_back(Student(id, name, pwd));
		else
			vTea.push_back(Teacher(id, name, pwd));

		ofs << id << " " << name << " " << pwd << endl;
		cout << "添加成功" << endl;
		ofs.close();

		system("pause");
		system("cls");		
	}

	void showAccount() 
	{
		cout << "请输入显示账号的类型" << endl;
		cout << "1.显示学生" << endl;
		cout << "2.显示教师" << endl;
		int select = 0;
		while (true)
		{
			cin >> select;
			if (select == 1)
			{
				for (auto it : vStu)
					cout << it.mID << " " << it.mName << " "<< it.mPwd <<endl;
				break;
			}
			else if (select == 2)
			{
				for (auto it : vTea)
					cout << it.mID << " " << it.mName << " " << it.mPwd << endl;
				break;
			}
			else
				cout << "请输入正确账号类型" << endl;
		}
		cout << "账号信息显示完毕" << endl;
		system("pause");
		system("cls");
	}

	void showComputer() 
	{
		for (auto it : vCom)
			cout << it.mID << " " << it.mCap << endl;
		system("pause");
		system("cls");
	}

	void cleanFile()
	{
		ofstream ofs(ORDER_FILE, ios::trunc);
		ofs.close();

		cout << "清空成功" << endl;
		system("pause");
		system("cls");
	}

	void intializeVec() 
	{
		vTea.clear();
		vStu.clear();

		ifstream ifs;
		ifs.open(STUDENT_FILE, ios::in);
		if (!ifs.is_open())
		{
			cout << "学生文件读取失败" << endl;
			return;
		}
		Student s;
		while (ifs >> s.mID && ifs >> s.mName && ifs >> s.mPwd) 
		{
			vStu.push_back(s);
		}
		cout << "当前学生人数为：" << vStu.size() << endl;
		ifs.close();

		ifs.open(TEACHER_FILE, ios::in);
		if (!ifs.is_open())
		{
			cout << "教师文件读取失败" << endl;
			return;
		} 
		Teacher t;
		while (ifs >> t.mID && ifs >> t.mName && ifs >> t.mPwd)
		{
			vTea.push_back(t);
		}
		cout << "当前教师人数为：" << vTea.size() << endl;
		ifs.close();
	}

	bool checkRepeat(int id, int type)
	{
		if (type == 1)
		{
			for (auto it : vStu)
			{
				if (id == it.mID)
					return true;
			}
		}
		else
		{
			for (auto it : vTea)
			{
				if (id == it.mID)
					return true;
			}
		}
		return false;
	}
};

void adminMenu(Identity* admin)
{
	Admin* man = (Admin*)admin;
	while (true)
	{
		admin->openMenu();
		int select = 0;
		cin >> select;

		switch (select)
		{
		case 1:
			man->addAccount();
			break;
		case 2:
			cout << "查看账号" << endl;
			man->showAccount();
			break;
		case 3:
			cout << "查看机房" << endl;
			man->showComputer();
			break;
		case 4:
			cout << "清空预约" << endl;
			man->cleanFile();
			break;
		case 0:
			delete admin;
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "输入有误，请重新输入" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void studentMenu(Identity* student)
{
	Student* stu = (Student*)student;
	while (true)
	{
		stu->openMenu();
		int select = 0;
		cin >> select;
		
		switch (select)
		{
		case 1:
			stu->applyOrder();
			break;
		case 2:
			cout << "我的预约" << endl;
			stu->showMyOrder();
			break;
		case 3:
			cout << "查看预约" << endl;
			stu->showAllOrder();
			break;
		case 4:
			cout << "取消预约" << endl;
			stu->cancelOrder();
			break;
		case 0:
			delete stu;
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "输入有误，请重新输入" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void teacherMenu(Identity* teacher)
{
	Teacher* tea = (Teacher*)teacher;
	while (true)
	{
		tea->openMenu();
		int select = 0;
		cin >> select;

		switch (select)
		{
		case 1:
			tea->showAllOrder();
			break;
		case 2:
			tea->validOrder();
			break;
		case 0:
			delete tea;
			cout << "注销成功" << endl;
			system("pause");
			system("cls");
			return;
		default:
			cout << "输入有误，请重新输入" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void loginIn(string fileName, int type)
{
	Identity* person = nullptr;
	ifstream ifs;
	ifs.open(fileName, ios::in);
	if (!ifs.is_open()) 
	{
		cout << "文件不存在" << endl;
		ifs.close();
		return;
	}
	int id = 0;
	string name;
	string pwd;

	if (type == 1) 
	{
		cout << "请输入学号：" << endl;
		cin >> id;
	}
	if (type == 2) 
	{
		cout << "请输入职工号：" << endl;
		cin >> id;
	}
	cout << "请输入姓名：" << endl;
	cin >> name;
	cout << "请输入密码：" << endl;
	cin >> pwd;

	if (type == 1) 
	{
		int fID;
		string fName;
		string fPwd;
		while (ifs >> fID && ifs >> fName && ifs >> fPwd)
		{
			if (id == fID && name == fName && pwd == fPwd) 
			{
				cout << "学生验证登陆成功" << endl;
				system("pause");
				system("cls");
				person = new Student(id, name, pwd);
				// 进入学生子菜单
				studentMenu(person);
				return;
			}
		}
	}
	else if (type == 2)
	{
		int fID;
		string fName;
		string fPwd;
		while (ifs >> fID && ifs >> fName && ifs >> fPwd)
		{
			if (id == fID && name == fName && pwd == fPwd)
			{
				cout << "教师验证登陆成功" << endl;
				system("pause");
				system("cls");
				person = new Teacher(id, name, pwd);
				// 进入教师子菜单
				teacherMenu(person);
				return;
			}
		}
	}
	else if (type == 3)
	{
		string fName;
		string fPwd;
		while (ifs >> fName && ifs >> fPwd)
		{
			if (name == fName && pwd == fPwd)
			{
				cout << "管理员验证登陆成功" << endl;
				system("pause");
				system("cls");
				person = new Admin(name, pwd);
				// 进入管理员子菜单
				adminMenu(person);
				return;
			}
		}
	}
	cout << "验证失败" << endl;
	system("pause");
	system("cls");
}

int main() 
{
	int select = 0;

	while (true)
	{
		cout << "请输入您的身份:" << endl;
		cout << "\t\t -----------------\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t|    1.学生       |\n";
		cout << "\t\t|    2.老师       |\n";
		cout << "\t\t|    3.管理员     |\n";
		cout << "\t\t|    0.退出       |\n";
		cout << "\t\t|                 |\n";
		cout << "\t\t -----------------\n";
		cout << "请输入您的选择：";
		cin >> select;

		switch (select)
		{
		case 1:
			loginIn(STUDENT_FILE, 1);
			break;
		case 2:
			loginIn(TEACHER_FILE, 2);
			break;
		case 3:
			loginIn(ADMIN_FILE, 3);
			break;
		case 0:
			cout << "欢迎下次使用" << endl;
			system("pause");
			return 0;
			break;
		default:
			cout << "输入有误，请重新输入" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
		return 0;

}