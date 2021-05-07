#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// �Զ���ѧ����
class Student {
public:
	int no = -1; // ���
	string id = ""; // ѧ��
	string name = ""; // ����
	double s1 = -1; // ����1
	double s2 = -1; // ����2
	double s3 = -1; // ����3
	double avg = -1; // ƽ����
};

// ȫ�ֱ���
const char binDBUrl[] = "./Resource/database/database.bin"; // bin���ݿ��ַ
const char txtDBUrl[] = "./Resource/database/database.txt"; // txt���ݿ��ַ
vector<Student> sList; // �����ݿ��ȡ��ѧ�������б�
int curIndex = -1; // ��¼��ǰ���
string curId = ""; // ��¼��ǰѧ��

/* ��ʾ���˵� */
void menu() {
	cout << "���˵���" << endl;
	cout << "------------------" << endl;
	cout << "1. ��ʾѧ����¼(list)" << endl;
	cout << "2. ����ѧ����¼(add)" << endl;
	cout << "3. ɾ��ѧ����¼(rem)" << endl;
	cout << "4. �޸�ѧ����¼(edit)" << endl;
	cout << "5. ���õ�ǰ��¼(set)" << endl;
	cout << "6. ����ѧ����¼(sort)" << endl;
	cout << "7. ����(help)" << endl;
	cout << "8. �˳�(quit)" << endl;
	cout << "------------------" << endl;
}



/********
**
**��BEGIN�����ߺ���
**
********/

/* �ж��ַ����Ƿ�Ϊ������ */
bool isDouble(string s) {
	if (s.empty()) return false;
	if (s.size() == 1 && !isdigit(s[0])) return false;
	if (s[0] != '-' && s[0] != '.' && !isdigit(s[0])) return false;
	if (s.size() == 2 && s[0] == '-' && s[1] == '.') return false;
	int dotCount = 0;
	if (s[0] == '.') dotCount++;
	if (s.size() >= 2)
		for (int i = 1; i < s.size(); i++) {
			if (s[i] == '.') dotCount++;
			else if (!isdigit(s[i])) return false;
		}
	if (dotCount >= 2) return false;
	return true;
}

/* �ж��ַ����Ƿ�Ϊ���� */
bool isInt(string s) {
	if (s.empty()) return false;
	if (s.size() == 1 && !isdigit(s[0])) return false;
	if (s[0] != '-' && !isdigit(s[0])) return false;
	if (s.size() >= 2)
		for (int i = 1; i < s.size(); i++) {
			if (!isdigit(s[i])) return false;
		}
	return true;
}

/* Ϊ�������鲹���������Ĳ��� */
/* args��ԭ�����б�cmdLen������ȣ�paraLen����Ҫ�������ȣ�msg����ʾ��Ϣ */
vector<string> reqPara(vector<string> args, int cmdLen, int paraLen, string msg) {
	if (args.size() < cmdLen + paraLen) {
		cout << msg;
		string paraLine, temp;
		getline(cin, paraLine);
		stringstream input(paraLine);
		while (input >> temp)
			args.push_back(temp);
	}
	return args;
}

/* �������б��к��е�ѧ����Ϣ�Ƿ�Ϸ� */
/* ���Ĳ�������Ϊ ѧ�� ���� �ɼ�1 �ɼ�2 �ɼ�3��begin: args�в�����ʼ���±� */
/* ѧ����������Ϊ�գ��ɼ�����Ϊ���ڵ���0�ĸ����� */
bool checkPara(vector<string> args, int begin) {
	string id = args[begin];
	string name = args[begin + 1];
	string s1Str = args[begin + 2];
	string s2Str = args[begin + 3];
	string s3Str = args[begin + 4];
	if (id == "" || name == "") return false;
	if (!isDouble(s1Str) || !isDouble(s2Str) || !isDouble(s3Str)) return false;
	double s1 = strtod(s1Str.c_str(), NULL);
	double s2 = strtod(s2Str.c_str(), NULL);
	double s3 = strtod(s3Str.c_str(), NULL);
	if (s1 < 0 || s2 < 0 || s3 < 0) return false;
	return true;
}

/* �ж�ѧ���Ƿ���� */
bool idExist(string id) {
	for (Student s : sList)
		if (id == s.id) return true;
	return false;
}

/* �ж��Ƿ����ѧ������ */
bool dataExist() {
	if (sList.empty()) {
		cout << "û�м�¼��" << endl;
		return false;
	}
	return true;
}

/* ���ݱ�������±�� */
void renumber() {
	int index = 0;
	// ע�⣺���ﲻ����foreachѭ�����������������쳣��
	for (int i = 0; i < sList.size(); i++)
		sList[i].no = ++index;
}

/* ���ݱ�����Զ�������ǰָ�� */
void setCur() {
	// �������ʱ����ָ��
	if (sList.empty()) {
		curIndex = -1;
		curId = "";
		return;
	}
	// sList�ǿղŻ�ִ��������߼�
	// ��ʼ��ʱ����Ĭ��ָ��
	if (curIndex == -1) {
		curIndex = sList[0].no;
		curId = sList[0].id;
		return;
	}
	// ɾ��������ָ��Խ��ʱ�ƶ�ָ��
	if (curIndex > sList.size()) {
		curIndex = sList.size();
		curId = sList[curIndex - 1].id;
		return;
	}
	// ����set���ü���ǰָ���¼��ɾ�����ƶ�ָ��
	if (curId != sList[curIndex - 1].id) {
		curId = sList[curIndex - 1].id;
	}
}

/* ���ݱ����Ѱ��ԭѧ������������ */
void setCurIndex() {
	for (int i = 0; i < sList.size(); i++) {
		if (curId == sList[i].id) {
			curIndex = sList[i].no;
			break;
		}
	}
}

/* ��ӡ�������� */
void print(vector<Student> list) {

	if (list.empty()) return;

	cout << "=======================================================================" << endl;
	cout << "||  ��� |      ѧ��|      ����|   �ɼ�1|   �ɼ�2|   �ɼ�3|  ƽ���ɼ�||" << endl;
	cout << "-----------------------------------------------------------------------" << endl;

	// ��ʽ���������
	for (Student s : list) {
		cout << "|| " << (s.id == curId ? "* " : "  ") << (s.no < 10 ? "0" : "") << s.no << "  |";
		cout << setw(10) << s.id << "|" << setw(10) << s.name << "|";
		cout << fixed << setprecision(2) << setw(8) << s.s1 << "|" << setw(8) << s.s2 << "|" << setw(8) << s.s3 << "|";
		cout << fixed << setprecision(2) << setw(10) << s.avg << "||" << endl;
	}

	cout << "=======================================================================" << endl;

}

/* �ȽϺ��� */
/* ����ȫΪ�������� */
bool cmpByAvg(Student a, Student b) {
	return a.avg < b.avg;
}
bool cmpById(Student a, Student b) {
	return a.id < b.id;
}
bool cmpByName(Student a, Student b) {
	return a.name < b.name;
}
bool cmpByS1(Student a, Student b) {
	return a.s1 < b.s1;
}
bool cmpByS2(Student a, Student b) {
	return a.s2 < b.s2;
}
bool cmpByS3(Student a, Student b) {
	return a.s3 < b.s3;
}

/********
**
**��END�����ߺ���
**
********/



/********
**
**��BEGIN�����ݿ������غ���
**
** ע���û��Ƿ���C��д��Ȩ��
** bin���ݿ��ʽ����ѧ�� ���� �ɼ�1 �ɼ�2 �ɼ�3 ƽ���ɼ�����һ�б�ʾһ����¼
** ʾ����08	yu	98	78	76	84
**
********/

/* ���ѧ������������Լ�� */
/* ��ű���Ϊ����0��������ѧ����������Ϊ�գ��ɼ�����Ϊ���ڵ���0�ĸ�������ƽ���ɼ�����������0.05 */
bool constraints(Student s) {
	if (s.no <= 0) return false;
	if (s.id == "" || s.name == "") return false;
	if (s.s1 < 0 || s.s2 < 0 || s.s3 < 0) return false;
	double avg = (s.s1 + s.s2 + s.s3) / 3.0;
	if (abs(s.avg - avg) > 0.05) return false;
	return true;
}

/* �־û� */
void persistence() {

	ofstream binDB, txtDB;

	// �����ݿ��ļ������
	binDB.open(binDBUrl, ios::trunc | ios::binary);
	txtDB.open(txtDBUrl, ios::trunc);

	// ��������д�����ݿ�
	txtDB << "��¼������" << endl;
	txtDB << sList.size() << endl;
	txtDB << "\n���\tѧ��\t����\t�ɼ�1\t�ɼ�2\t�ɼ�3\tƽ���ɼ�" << endl;

	int index = 0; // ���
	for (Student s : sList) {
		s.no = ++index;
		binDB << s.id << "\t" << s.name << "\t" << s.s1 << "\t" << s.s2 << "\t" << s.s3 << "\t" << s.avg << endl;
		txtDB << s.no << "\t" << s.id << "\t" << s.name << "\t" << fixed << setprecision(2) << s.s1 << "\t" << s.s2 << "\t" << s.s3 << "\t" << s.avg << endl;
	}

	binDB.close();
	txtDB.close();

}

/* �½����ݿ� */
void createDB(const char url[]) {
	ofstream createDB;
	createDB.open(url, ios::binary);
	if (!createDB) {
		cout << "���ݿ⡰" << url << "�������ڣ����Զ�����ʧ��\n�����û��Ƿ���C��д��Ȩ�ޣ�" << endl;
		system("pause");
		exit(0);
	}
	else cout << "���ݿ⡰" << url << "�������ڣ����Զ�������" << endl;
	createDB.close();
}

/* ��ʼ�����ݿ� */
void DBUtil() {

	ifstream binDB, txtDB;

	// �����ݿ��ļ�
	binDB.open(binDBUrl, ios::binary);
	txtDB.open(txtDBUrl);

	// ���ݿⲻ�������½����ݿ�
	if (!binDB) {
		createDB(binDBUrl);
		binDB.open(binDBUrl, ios::binary);
	}
	if (!txtDB) {
		createDB(txtDBUrl);
		txtDB.open(txtDBUrl);
	}

	// �����ݿ�����ݶ�ȡ������
	int no = 0;
	while (!binDB.eof()) {
		string dataLine; // �����У���ʱ�ַ���
		Student student;
		getline(binDB, dataLine);
		stringstream input(dataLine);
		if (dataLine != "") {
			student.no = ++no;
			input >> student.id >> student.name >> student.s1 >> student.s2 >> student.s3 >> student.avg;
			if (constraints(student))
				sList.push_back(student);
			else {
				cout << "���ݿ��ļ��𻵻������쳣���������ݿ������ݵ������ԣ�" << endl;
				system("pause");
				exit(0);
			}
		}
	}
	binDB.close();
	txtDB.close();

	persistence(); // ��֤txt���ݿ�������bin���ݿ�һ��

}

/********
**
**��END�����ݿ������غ���
**
********/



/********
**
**��BEGIN��1��list����������غ���
**
********/

/* ��ӡ��ѯ��Ϣ */
void printQuery(vector<string> args, vector<Student> res) {

	print(res);

	if (args.size() <= 1) return;

	if (args[1] == "a") {
		cout << "\t���м�¼��Ŀ��" << res.size() << endl;
		cout << "\t��ǰ��¼��ţ�" << curIndex << endl;
	}
	else if (args[1] == "n") {
		cout << "�鵽����Ϊ��" << args[2] << "���ļ�¼ " << res.size() << " ��" << endl;
	}
	else if (args[1] == "x")
		if (res.size() == 0) cout << "����ѧ�Ų����ڣ�" << endl;

}

/* ��ѯ��ǰ��¼ */
vector<Student> queryCur() {
	vector<Student> res;
	for (Student s : sList) {
		if (curId == s.id) {
			res.push_back(s);
			break;
		}
	}
	return res;
}

/* ����ѧ�Ų�ѯѧ�� */
vector<Student> queryById(string id) {
	vector<Student> res;
	for (Student s : sList) {
		if (id == s.id) {
			res.push_back(s);
			break;
		}
	}
	return res;
}

/* ����������ѯѧ�� */
vector<Student> queryByName(string name) {
	vector<Student> res;
	for (Student s : sList)
		if (name == s.name) res.push_back(s);
	return res;
}

/* 1. ��ʾѧ����¼ */
void list(vector<string> args) {
	vector<Student> res;
	// list����ʾ��ǰ��¼
	if (args.size() <= 1) {
		if (!dataExist()) return;
		res = queryCur();
	}
	else {
		// list a����ʾ���м�¼
		if (args[1] == "a") {
			if (!dataExist()) return;
			res = sList;
		}
		// list x ѧ��ֵ����ʾָ��ѧ�ż�¼�������ı䵱ǰ��¼
		else if (args[1] == "x") {
			// if (!dataExist()) return; // ���ղο���������û�н�������Ϊ���ж�
			args = reqPara(args, 2, 1, "������ѧ�ţ�");
			if (args.size() < 3) return; // δ����ѧ���򷵻ؿ���̨
			res = queryById(args[2]);
		}
		// list n ����ֵ����ʾ���С�����=���ļ�¼�������ı䵱ǰ��¼
		else if (args[1] == "n") {
			if (!dataExist()) return;
			args = reqPara(args, 2, 1, "������������");
			if (args.size() < 3) return; // δ���������򷵻ؿ���̨
			res = queryByName(args[2]);
		}
		// list ��������
		else {
			cout << "�����������ȷ��" << endl;
			return;
		}
	}
	printQuery(args, res);
}

/********
**
**��END��1��list����������غ���
**
********/



/********
**
**��BEGIN��2��add����������غ���
**
********/

/* ��ӡ�����Ϣ */
void printAdd() {
	cout << "���������¼�¼��" << endl;
	// ִ��һ��list����ӡ�����Ϣ
	list({ "list" });
}

/* ����ѧ����¼ */
/* begin��args�в�����ʼ���±� */
bool insert(vector<string> args, int begin) {

	// ����Ϸ��򴴽��µ�ѧ������
	Student s;
	s.no = sList.size() + 1;
	s.id = args[begin];
	s.name = args[begin + 1];
	s.s1 = strtod(args[begin + 2].c_str(), NULL);
	s.s2 = strtod(args[begin + 3].c_str(), NULL);
	s.s3 = strtod(args[begin + 4].c_str(), NULL);
	s.avg = (s.s1 + s.s2 + s.s3) / 3.0;

	// ������ѧ����������������ݿ⣬������Ϊ��ǰ����
	sList.push_back(s);
	curIndex = s.no;
	curId = s.id;
	persistence();

	return true;

}

/* 2. ����ѧ����¼ */
void add(vector<string> args) {

	// add ������ʾ����
	if (args.size() <= 1) {
		args = reqPara(args, 1, 5, "�������¼��");
		if (args.size() < 6 || !checkPara(args, 1)) {
			cout << "������󣡼�¼��ʽΪ��ѧ�� ���� �ɼ�1 �ɼ�2 �ɼ�3" << endl;
			return;
		}
		else {
			if (idExist(args[1])) {
				cout << "����ѧ���Ѵ��ڣ��������������룡" << endl;
				return;
			}
			insert(args, 1);
		}
	}
	else {
		// add a ѧ�� ���� ����1 ����2 ����3
		if (args[1] == "a") {
			if (args.size() < 7 || !checkPara(args, 2)) {
				cout << "��add a�����������ʽΪ��add a ѧ�� ���� �ɼ�1 �ɼ�2 �ɼ�3" << endl;
				return;
			}
			if (idExist(args[2])) {
				cout << "����ѧ���Ѵ��ڣ��������������룡" << endl;
				return;
			}
			insert(args, 2);
		}
		// add �������
		else {
			cout << "�����������ȷ��" << endl;
			return;
		}
	}

	printAdd();

}

/********
**
**��END��2��add����������غ���
**
********/



/********
**
**��BEGIN��3��rem����������غ���
**
** ����ʾ�����У��ò��ֵ�������ʾ���ڻ��ҺͲ�һ��
** �������ʾ������һЩ�޸ĺͲ��䣬��֤��ʾ��ͳһ�ͽ������Ѻ�
**
********/

/* ȷ��ɾ����Ϣ */
bool confirmRm(string msg) {
	cout << msg;
	char cfm = getchar();
	// ������뻺��
	string garbage;
	getline(cin, garbage);
	// ���ݲο�����ֻ��'y*'���ж�Ϊȷ��
	if (cfm == 'y') return true;
	return false;
}

/* ����ѧ��ɾ��ѧ�� */
void rmById(string id) {
	if (!dataExist()) return;
	// ����ѧ�Ų���ѧ�����±�
	int index = -1;
	for (int i = 0; i < sList.size(); i++) {
		if (id == sList[i].id) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		cout << "ѧ�������ڣ�" << endl;
		return;
	}
	cout << "��Ҫɾ����ѧ����ϢΪ��" << endl;
	// ִ��һ��list x����ӡҪɾ����ѧ����Ϣ
	list({ "list", "x", id });
	if (confirmRm("��ȷ��Ҫɾ����(y/n)��")) {
		sList.erase(sList.begin() + index); // �ӳ���������ɾ����Ӧѧ��
		renumber(); // ���±��
		persistence(); // �־û�
		setCur(); // ����ָ��
		cout << "ɾ���ɹ���" << endl;
	}
	else cout << "ɾ��ʧ�ܣ�" << endl;
}

/* ɾ��ȫ��ѧ�� */
void rmAll() {
	if (!dataExist()) return;
	if (confirmRm("��ȷ��Ҫɾ�����м�¼��(y/n)��")) {
		sList.clear();
		setCur();
		persistence();
		cout << "ɾ���ɹ���" << endl;
	}
	else cout << "ɾ��ʧ�ܣ�" << endl;
}

/* 3. ɾ��ѧ����¼ */
void remove(vector<string> args) {

	// rem��ɾ����ǰ��¼
	if (args.size() <= 1) rmById(curId);
	else {
		// rem a��ɾ�����м�¼
		if (args[1] == "a") rmAll();
		// rem x ѧ��ֵ��ɾ��ָ��ѧ�Ŷ�Ӧ��¼
		else if (args[1] == "x") {
			if (!dataExist()) return;
			args = reqPara(args, 2, 1, "������Ҫɾ����ѧ����ѧ�ţ�");
			if (args.size() < 3) return;
			rmById(args[2]);
		}
		// rem �������
		else {
			cout << "�����������ȷ��" << endl;
			return;
		}
	}

}

/********
**
**��END��3��rem����������غ���
**
********/



/********
**
**��BEGIN��4��edit����������غ���
**
** ����ʾ�����У��ò��ֵ�������ʾ���ڴ���
** �������ʾ����������
**
********/

/* ��ӡ�༭��Ϣ */
void printUpdate(int flag) {
	cout << "�޸�" << (flag == 0 ? "ǰ" : "��") << "��¼Ϊ:" << endl;
	// ִ��һ��list
	list({ "list" });
}

/* ����ѧ����Ϣ */
/* begin��args�в�����ʼ���±� opt����Ҫ�޸ĵĲ���λ�ã�����0��ʾ�޸�ȫ�������� */
void update(vector<string> args, int begin, int opt) {

	if (opt == 0) {
		sList[curIndex - 1].id = args[begin];
		curId = args[begin];
		sList[curIndex - 1].name = args[begin + 1];
		sList[curIndex - 1].s1 = strtod(args[begin + 2].c_str(), NULL);
		sList[curIndex - 1].s2 = strtod(args[begin + 3].c_str(), NULL);
		sList[curIndex - 1].s3 = strtod(args[begin + 4].c_str(), NULL);
		sList[curIndex - 1].avg = (sList[curIndex - 1].s1 + sList[curIndex - 1].s2 + sList[curIndex - 1].s3) / 3.0;
	}
	else if (opt == 1) {
		sList[curIndex - 1].id = args[begin];
		curId = args[begin];
	}
	else if (opt == 2) sList[curIndex - 1].name = args[begin];
	else if (opt == 3) sList[curIndex - 1].s1 = strtod(args[begin].c_str(), NULL);
	else if (opt == 4) sList[curIndex - 1].s2 = strtod(args[begin].c_str(), NULL);
	else if (opt == 5) sList[curIndex - 1].s3 = strtod(args[begin].c_str(), NULL);
	if (opt == 3 || opt == 4 || opt == 5) sList[curIndex - 1].avg = (sList[curIndex - 1].s1 + sList[curIndex - 1].s2 + sList[curIndex - 1].s3) / 3.0;

	persistence();

}

/* �޸ĵ�ǰѧ����ȫ����Ϣ */
/* begin: args�в�����ʼ���±� */
void editAll(vector<string> args, int begin) {
	if (!dataExist()) return;
	if (begin == 2 && (args.size() < 7 || !checkPara(args, begin))) {
		cout << "��edit a�����������ʽΪ��edit a ѧ�� ���� �ɼ�1 �ɼ�2 �ɼ�3" << endl;
		return;
	}
	printUpdate(0);
	args = reqPara(args, begin, 5, "�������¼��");
	if (begin == 1 && (args.size() < 6 || !checkPara(args, begin))) {
		cout << "������󣡼�¼��ʽΪ��ѧ�� ���� �ɼ�1 �ɼ�2 �ɼ�3" << endl;
		return;
	}
	else {
		if (idExist(args[begin]) && args[begin] != curId) {
			cout << "����ѧ���Ѵ��ڣ��������ظ�ѧ�ţ�" << endl; // ѧ�Ų�������ѧ���ظ������ɼ���Ϊ��ǰѧ��
			return;
		}
		update(args, begin, 0);
		printUpdate(1);
	}
}

/* �޸ĵ�ǰѧ����ѧ�� */
void editId(vector<string> args) {
	if (!dataExist()) return;
	printUpdate(0);
	args = reqPara(args, 2, 1, "������ѧ����ֵ��");
	if (args.size() < 3) return;
	if (idExist(args[2]) && args[2] != curId) {
		cout << "����ѧ���Ѵ��ڣ������޸ģ�" << endl;
		return;
	}
	update(args, 2, 1);
	printUpdate(1);
}

/* �޸ĵ�ǰѧ�������� */
void editName(vector<string> args) {
	if (!dataExist()) return;
	printUpdate(0);
	args = reqPara(args, 2, 1, "������������ֵ��");
	if (args.size() < 3) return;
	if (args[2] != "") {
		update(args, 2, 2);
		printUpdate(1);
	}
}

/* �޸ĵ�ǰѧ���ķ��� */
void editScore(vector<string> args) {
	if (!dataExist()) return;

	int opt = 0;
	if (args[1] == "s1") opt = 1;
	else if (args[1] == "s2") opt = 2;
	else opt = 3; // args[1] == "s3"

	printUpdate(0);
	args = reqPara(args, 2, 1, "���������" + to_string(opt) + "��ֵ��");
	if (args.size() < 3) return;
	if (isDouble(args[2])) {
		update(args, 2, opt + 2);
		printUpdate(1);
	}
	else cout << "������Ϣ����δ���¡�" << endl;
}

/* 4. �޸�ѧ����¼ */
void edit(vector<string> args) {
	// edit���޸ĵ�ǰ��¼����Ҫ����������
	if (args.size() <= 1) editAll(args, 1);
	else {
		// edit a ѧ��ֵ ����ֵ ���� 1 ֵ ���� 2 ֵ ���� 3 ֵ���޸ĵ�ǰ��¼������
		if (args[1] == "a") editAll(args, 2);
		// edit x ѧ��ֵ��ֻ�޸ĵ�ǰ��¼ѧ��
		else if (args[1] == "x") editId(args);
		// edit n ����ֵ��ֻ�޸ĵ�ǰ��¼����
		else if (args[1] == "n") editName(args);
		// edit s1 ���� 1 ֵ��ֻ�޸ĵ�ǰ��¼���� 1
		// edit s2 ���� 2 ֵ��ֻ�޸ĵ�ǰ��¼���� 2
		// edit s3 ���� 3 ֵ��ֻ�޸ĵ�ǰ��¼���� 3
		else if (args[1] == "s1" || args[1] == "s2" || args[1] == "s3") editScore(args);
		// edit �������
		else cout << "�����������ȷ��" << endl;
	}
}

/********
**
**��END��4��edit����������غ���
**
********/



/********
**
**��BEGIN��5��set����������غ���
**
** ����ʾ�����У��ò��ִ��ڲ����ж������������������
**
** ����ʾ�����У��ò��ֵ�������ʾ���ڻ��ҺͲ�һ��
** �������ʾ������һЩ�޸ģ���֤��ʾ��ͳһ�ͽ������Ѻ�
**
********/

/* 5. ���õ�ǰ��¼ */
void set(vector<string> args) {

	// set ������ȫ
	if (args.size() <= 1) {
		cout << "������󣡡�set�����������ʽΪ��set ���ֵ" << endl;
		return;
	}

	// set ���ֵ�����õ�ǰ��¼Ϊ������ڵļ�¼����ʾ
	// �ж��Ƿ�������
	if (!dataExist()) return;

	// ��֤�����Ƿ�Ϊ����
	if (!isInt(args[1])) {
		cout << "������ʽ����ȷ��" << endl;
		return;
	}

	int newNo = atoi(args[1].c_str());
	// ��֤����Ƿ�Խ��
	if (newNo <= 0 || newNo > sList.size()) {
		cout << "�������Ų��ڷ�Χ�ڣ�" << endl;
		return;
	}

	curIndex = newNo;
	setCur();
	cout << "���óɹ���" << endl;

	// ִ��һ��list����ӡ��ǰָ�����
	list({ "list" });

}

/********
**
**��END��5��set����������غ���
**
********/



/********
**
**��BEGIN��6��sort����������غ���
**
********/

/* 6. ����ѧ����¼ */
void sort(vector<string> args) {

	// sort������ƽ��������
	if (args.size() <= 1) {
		if (!dataExist()) return;
		sort(sList.begin(), sList.end(), cmpByAvg);
	}
	else {
		// sort x������ѧ������
		if (args[1] == "x") {
			if (!dataExist()) return;
			sort(sList.begin(), sList.end(), cmpById);
		}
		// sort n��������������
		else if (args[1] == "n") {
			if (!dataExist()) return;
			sort(sList.begin(), sList.end(), cmpByName);
		}
		// sort s1�����ݷ��� 1 ����
		else if (args[1] == "s1") {
			if (!dataExist()) return;
			sort(sList.begin(), sList.end(), cmpByS1);
		}
		// sort s2�����ݷ��� 2 ����
		else if (args[1] == "s2") {
			if (!dataExist()) return;
			sort(sList.begin(), sList.end(), cmpByS2);
		}
		// sort s3�����ݷ��� 3 ����
		else if (args[1] == "s3") {
			if (!dataExist()) return;
			sort(sList.begin(), sList.end(), cmpByS3);
		}
		// sort ��������
		else {
			cout << "�����������ȷ��" << endl;
			return;
		}
	}

	renumber(); // ���±��
	setCurIndex(); // Ѱ��ԭָ���Ӧ�������
	persistence(); // �־û������ݲο����������Ӱ�����ݿ����ݣ�
	setCur(); // ����ָ��

	// ִ��һ��list a
	list({ "list", "a" });

}

/********
**
**��END��6��sort����������غ���
**
********/



/********
**
**��BEGIN��7��help����������غ���
**
********/

/* 7. ���� */
void help(vector<string> args) {

	if (args.size() <= 1) {
		menu();
		return;
	}

	if (args[1] == "list") {
		cout << "\tlist---------------��ʾ��ǰ��¼" << endl;
		cout << "\tlist a-------------��ʾ���м�¼" << endl;
		cout << "\tlist x ѧ��ֵ------��ʾ��Ӧ��ѧ�š���¼" << endl;
		cout << "\tlist n ����ֵ------��ʾ������Ӧ����������¼" << endl;
	}
	else if (args[1] == "add") {
		cout << "\tadd----------------------------------��������һ���¼�¼" << endl;
		cout << "\tadd a ѧ�� ���� ����1 ����2 ����3----��������һ���¼�¼�����٣�" << endl;
	}
	else if (args[1] == "rem") {
		cout << "\trem----------------ɾ����ǰ��¼" << endl;
		cout << "\trem a--------------ɾ�����м�¼" << endl;
		cout << "\trem x ѧ��ֵ-------ɾ��ָ����ѧ�š���¼" << endl;
	}
	else if (args[1] == "edit") {
		cout << "\tedit---------------���µ�ǰ��¼������" << endl;
		cout << "\tedit x  ѧ��ֵ-----���µ�ǰ��¼ѧ��ֵ" << endl;
		cout << "\tedit n  ����ֵ-----���µ�ǰ��¼����ֵ" << endl;
		cout << "\tedit s1 ����1ֵ----���µ�ǰ��¼����1ֵ" << endl;
		cout << "\tedit s2 ����2ֵ----���µ�ǰ��¼����2ֵ" << endl;
		cout << "\tedit s3 ����3ֵ----���µ�ǰ��¼����3ֵ" << endl;
		cout << "\tedit a ѧ�� ���� ����1 ����2 ����3----���µ�ǰ��¼��������٣�" << endl;
	}
	else if (args[1] == "set") {
		cout << "\tset ���ֵ---------ʹ�á���š����ж�λ��������Ϊ��ǰ��¼" << endl;
	}
	else if (args[1] == "sort") {
		cout << "\tsort---------------ʹ�á�ƽ���ɼ�����������" << endl;
		cout << "\tsort x-------------ʹ�á�ѧ�š���������" << endl;
		cout << "\tsort n-------------ʹ�á���������������" << endl;
		cout << "\tsort s1------------ʹ�á�����1����������" << endl;
		cout << "\tsort s2------------ʹ�á�����2����������" << endl;
		cout << "\tsort s3------------ʹ�á�����3����������" << endl;
	}
	else
		cout << "û�С�" << args[1] << "���İ�����" << endl;

}

/********
**
**��END��7��help����������غ���
**
********/



/* �������������� */
void cmd(vector<string> args) {

	if (args.size() <= 0)
		return;

	if (args[0] == "list")
		list(args);
	else if (args[0] == "add")
		add(args);
	else if (args[0] == "rem")
		remove(args);
	else if (args[0] == "edit")
		edit(args);
	else if (args[0] == "set")
		set(args);
	else if (args[0] == "sort")
		sort(args);
	else if (args[0] == "help")
		help(args);
	else if (args[0] == "quit")
		exit(0);
	else if (args[0] == "clear") // �����ã�����
		system("cls");
	else
		cout << "��ϵͳ���" << endl;

}

/* main������������� */
int main() {

	system("title ѧ���ɼ�����ϵͳ"); // ���ñ���
	system("color 4E"); // ���ñ�����������ɫ����ɫ��4��������ɫ��E��

	DBUtil(); // ��ʼ�����ݿ⣬�����ݶ������
	setCur(); // ��ʼ��ָ��
	menu();	// ��ʾ���˵�

	// ��ʼ���ղ�������
	while (true) {

		cout << ">>:";

		string cmdLine, buffer; // ���������У������ַ���
		vector<string> cmdArgs; // ��������

		cin.clear();
		getline(cin, cmdLine);
		stringstream input(cmdLine);
		while (input >> buffer)
			cmdArgs.push_back(buffer);

		// �����������
		cmd(cmdArgs);

	}

	return 0;

}
