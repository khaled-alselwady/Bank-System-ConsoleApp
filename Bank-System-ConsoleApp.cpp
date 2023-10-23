#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <windows.h>
#include <cstdlib>


using namespace std;

const string ClientsFileName = "Clients.txt";

const string UsersFileName = "Users.txt";

enum enMainMenuOption { Show = 1, Add, Delete, Update, Find, Transactions, ManageUsers, ATMSystem, Logout };

enum enTransactionsMenuOption { Deposit = 1, Withdraw, TotalBalances, MainMenu };

enum enManageUsersMenuOption { ListUsers = 1, AddUsers, DeleteUesrs, UpdateUsers, FindUsers, Main };

enum enMainMenuPermissions {
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64, pATMSystem = 128
};

enum enATMMainMenuOption { QuickWithdraw = 1, NormalWithdraw, ATMDeposit, ATMTotalBalances, ChangePinCode, Mainn };

struct strClients
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string NumberPhone = "";
	double AcountBalance = 0;
	bool MarkToDelete = false;
};

struct strUsers
{
	string UserName = "";
	string Password = "";
	int Permissions = 0;
	string SecurityCode = "";
	bool MarkToDelete = false;
};

strUsers CurrentUser;
strClients CurrentClient;

void ShowMainMenu();
void ShowTransactionsMenuScreen();
void ShowManageUsersScreen();
void PerformManageUsersMenuOption(enManageUsersMenuOption);
void Login();
void ShowATMMainMenuScreen();
void LoginATMSystem();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();

//void PrintLine(int Length)
//{
//    cout << "\t\t\t\t\t\t\t\t";
//    cout.fill(196);
//    cout.width(Length);
//    cout << (char)196;
//    cout << endl;
//}

void ChangeColour(short NumberOfColour)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, NumberOfColour);
}

short RandomNumber(short From, short To)
{
	short Random = rand() % (To - From + 1) + From;
	return Random;
}

string GetNewSecurityCode()
{
	string characters = "1234567890ABCDEFG1234567890HIJKLMNO1234567890PQRSTUVWXYZ1234567890abcdefghijklmno1234567890pqrstuv1234567890wxyz1234567890";

	string NewSecurityCode = "";

	for (int i = 0; i < 6; i++) // I want the length of the SecurityCode = 6
	{
		NewSecurityCode += characters[RandomNumber(0, characters.length() - 1)];
	}

	return NewSecurityCode;
}

void PrintClientRecord(strClients ClientData)
{
	cout << "\t\t| " << setw(15) << left << ClientData.AccountNumber;
	cout << "\t\t| " << setw(10) << left << ClientData.PinCode;
	cout << "\t\t| " << setw(40) << left << ClientData.Name;
	cout << "\t\t| " << setw(12) << left << ClientData.NumberPhone;
	cout << "\t\t| " << setw(12) << left << ClientData.AcountBalance;
}

void PrintUersRecord(strUsers User)
{
	cout << "\t\t| " << setw(15) << left << User.UserName;
	cout << "\t\t| " << setw(10) << left << User.Password;
	cout << "\t\t| " << setw(15) << left << User.Permissions;
}

void PrintClientRecordBalanceLine(strClients ClientData)
{
	cout << "\t\t| " << setw(15) << left << ClientData.AccountNumber;
	cout << "\t\t| " << setw(40) << left << ClientData.Name;
	cout << "\t\t| " << setw(12) << left << ClientData.AcountBalance;
}

vector<string> SplitString(string PrintLine, string Seperetor = "#//#")
{
	vector<string> vString;

	short Position = 0;
	string Word = "";

	while ((Position = PrintLine.find(Seperetor)) != PrintLine.npos)
	{
		Word = PrintLine.substr(0, Position);

		if (Word != "")
		{
			vString.push_back(Word);
		}

		PrintLine.erase(0, Word.length() + Seperetor.length());
	}

	if (PrintLine != "")
	{
		vString.push_back(PrintLine);
	}

	return vString;
}

strClients ConvertLineClientToRecord(string PrintLine, string Seperetor = "#//#")
{
	strClients Client;
	vector<string> vString = SplitString(PrintLine, Seperetor);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.NumberPhone = vString[3];
	Client.AcountBalance = stod(vString[4]);

	return Client;
}

strUsers ConvertLineUsersToRecord(string PrintLine, string Seperetor = "#//#")
{
	strUsers Users;
	vector<string> vString = SplitString(PrintLine, Seperetor);

	Users.UserName = vString[0];
	Users.Password = vString[1];
	Users.Permissions = stoi(vString[2]);
	Users.SecurityCode = vString[3];

	return Users;
}

string ConvertRecordClientToLine(strClients Clients, string Seperator = "#//#")
{
	string PrintLine = "";

	PrintLine += Clients.AccountNumber + Seperator;
	PrintLine += Clients.PinCode + Seperator;
	PrintLine += Clients.Name + Seperator;
	PrintLine += Clients.NumberPhone + Seperator;
	PrintLine += to_string(Clients.AcountBalance);

	return PrintLine;
}

vector<strClients> SaveCleintsDataToFile(string FileName, vector<strClients> vClient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine = "";

	if (MyFile.is_open())
	{

		for (strClients& Clients : vClient)
		{
			if (Clients.MarkToDelete == false)
			{
				DataLine = ConvertRecordClientToLine(Clients);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}

	return vClient;
}

vector <strClients> LoadCleintsDataFromFile(string FileName)
{
	vector<strClients> vClient;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string PrintLine = "";
		strClients Client;

		while (getline(MyFile, PrintLine))
		{
			Client = ConvertLineClientToRecord(PrintLine);
			vClient.push_back(Client);
		}

		MyFile.close();
	}

	return vClient;
}

vector <strUsers> LoadUsersDataFromFile(string FileName)
{
	vector<strUsers> vUsers;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string PrintLine = "";
		strUsers Users;

		while (getline(MyFile, PrintLine))
		{
			Users = ConvertLineUsersToRecord(PrintLine);
			vUsers.push_back(Users);
		}

		MyFile.close();
	}

	return vUsers;
}

void AmountExceeds()
{
	ChangeColour(4);
	cout << "\a";
	cout << "\n\t\t\t\t\t\t\t\tThe amount exceeds your balance !";
	ChangeColour(7);
	cout << "\n\n\t\t\t\t\t\t\t\tPlease make another choice.\n\n";
	cout << "\t\t\t\t\t\t\t\tPress Any Key to continue...";
	system("pause>0");
}

short ReadQuickWithdrawChoice()
{
	short Choice = 0;
	cout << "\n\t\t\t\t\t\t\t\tChoose what to withdraw from [1] to [8] ? ";
	cin >> Choice;

	return Choice;
}

string ConvertRecordUserToLine(strUsers User, string Seperator = "#//#")
{
	return User.UserName + Seperator + User.Password + Seperator + to_string(User.Permissions) + Seperator + User.SecurityCode;
}

vector<strUsers> SaveUsersDataToFile(string FileName, vector<strUsers> vUser)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine = "";

	if (MyFile.is_open())
	{

		for (strUsers& Users : vUser)
		{
			if (Users.MarkToDelete == false)
			{
				DataLine = ConvertRecordUserToLine(Users);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}

	return vUser;
}

bool ChangePinCodeToFileByOldPinCode(string OldPinCode, string NewPinCode, vector <strClients>& vClients)
{
	char Answer = ' ';

	cout << "\n\n\t\t\t\t\t\t\t\tAre you sure you want to change your PinCode? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (strClients& C : vClients)
		{
			if (C.PinCode == OldPinCode)
			{
				C.PinCode = NewPinCode;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				ChangeColour(10);
				cout << "\n\t\t\t\t\t\t\t\tDone successfully !";
				ChangeColour(7);
				cout << " New PinCode is : " << C.PinCode;

				return true;
			}

		}

		return false;
	}
}

bool ChangePasswordToFileBySecurityCode(string SecurityCode, string NewPassword, vector <strUsers>& vUser)
{

	for (strUsers& U : vUser)
	{
		if (U.SecurityCode == SecurityCode)
		{
			U.Password = NewPassword;
			U.SecurityCode = GetNewSecurityCode();
			CurrentUser.SecurityCode = U.SecurityCode;
			SaveUsersDataToFile(UsersFileName, vUser);
			ChangeColour(10);
			cout << "\n\t\t\t\t\t\t\t\tDone successfully !";
			ChangeColour(7);
			cout << " New Security Code is : " << CurrentUser.SecurityCode;
			cout << "\n\n\t\t\t\t\t\t\t\tPress any key to login again.\n";
			cout << "\n\t\t\t\t\t\t\t\t";
			system("pause>0");
			return true;
		}
	}

	return false;

}

void GoBackToATMMainScreen()
{
	cout << "\n\n\t\t\t\t\t\t\t\tPress any key to go back to ATM Main Menu...";
	system("pause>0");
	cout << "\a";
	ShowATMMainMenuScreen();
}

short ReadATMMainMenuOption()
{
	short Choice = 0;
	cout << "\n\t\t\t\t\t\t\t\tChoose what do want to do? [1 to 6]? ";
	cin >> Choice;

	return Choice;
}

string ReadPinCode()
{
	string Password = "";
	cout << "\n\t\t\t\t\t\t\t\tEnter Password? ";
	cin >> Password;
	return Password;
}

void GoBackToMainScreen()
{
	cout << "\n\t\t\t\t\t\t\t\tPress any key to go back to Main Menu...";
	system("pause>0");
	cout << "\a";
	ShowMainMenu();
}

void GoBackToTransactionScreen()
{
	cout << "\n\n\t\t\t\t\t\t\t\tPress any key to go back to Transaction Menu...";
	system("pause>0");
	cout << "\a";
	ShowTransactionsMenuScreen();
}

void GoBackToManageUsersScreen()
{
	cout << "\n\t\t\t\t\t\t\t\tPress any key to go back to Manage Users Menu...";
	system("pause>0");
	cout << "\a";
	ShowManageUsersScreen();
}

short ReadMainMenuOption()
{
	short Choice = 0;
	cout << "\t\t\t\t\t\t\t\tChoose what do want to do? [1 to 9]? ";
	cin >> Choice;

	return Choice;
}

short ReadTransactionsMenuOption()
{
	short Choice = 0;
	cout << "\n\t\t\t\t\t\t\t\tChoose what do want to do? [1 to 4]? ";
	cin >> Choice;

	return Choice;
}

short ReadManageUsersMenuOption()
{
	short Choice = 0;
	cout << "\n\t\t\t\t\t\t\t\tChoose what do want to do? [1 to 6]? ";
	cin >> Choice;

	return Choice;
}

string ReadUserName()
{
	string Name = "";
	cout << "\n\t\t\t\t\t\t\t\tEnter Username? ";
	getline(cin >> ws, Name);
	return Name;
}

string ReadPassword()
{
	string Password = "";
	cout << "\n\t\t\t\t\t\t\t\tEnter Password? ";
	cin >> Password;
	return Password;
}

bool FindClientByAccountNumber(vector<strClients>& vClinet, strClients& Clients, string AccountNumber)
{
	for (strClients& C : vClinet)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Clients = C;
			return 1;
		}
	}

	return 0;
}

bool FindUserByUsername(vector<strUsers>& vUsers, strUsers& Users, string Username)
{
	for (strUsers& U : vUsers)
	{
		if (U.UserName == Username)
		{
			Users = U;
			return 1;
		}
	}

	return 0;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <strClients>& vClients)
{
	for (strClients& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkToDelete = true;
			return 1;
		}
	}

	return 0;
}

bool MarkUserForDeleteByUsername(string Username, vector <strUsers>& vUsers)
{
	for (strUsers& U : vUsers)
	{
		if (U.UserName == Username)
		{
			U.MarkToDelete = true;
			return 1;
		}
	}

	return 0;
}

void PrintClientCard(strClients Clients)
{
	//cout << "\n\t\t\t\t\t\t\t\tThe following are the client details:\n";
	ChangeColour(9);
	cout << "\n\n\t\t\t\t\t\t\t\t_____________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t\t\t Client Info\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t_____________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t| Account Number         | " << Clients.AccountNumber << endl;
	cout << "\n\t\t\t\t\t\t\t\t| Pin Code               | " << Clients.PinCode << endl;
	cout << "\n\t\t\t\t\t\t\t\t| Name                   | " << Clients.Name << endl;
	cout << "\n\t\t\t\t\t\t\t\t| Phone                  | " << Clients.NumberPhone << endl;
	cout << "\n\t\t\t\t\t\t\t\t| Account Balance        | " << Clients.AcountBalance << endl;
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t_____________________________________________________________\n";
	ChangeColour(7);
}

void PrintUserCard(strUsers User)
{
	//cout << "\nThe following are the user details:\n";

	ChangeColour(9);
	cout << "\n\n\t\t\t\t\t\t\t\t_____________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t\t\t User Info\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t_____________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t| Username                | " << User.UserName << endl;
	cout << "\n\t\t\t\t\t\t\t\t| Password                | " << User.Password << endl;
	cout << "\n\t\t\t\t\t\t\t\t| Permissions             | " << User.Permissions << endl;
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t_____________________________________________________________\n";
	ChangeColour(7);
}

int ReadPermissionsToSet()
{
	char Yes_No = ' ';
	int Permissions = 0;

	cout << "\n\n\t\t\t\t\t\t\t\tDo you want to give full access? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		return -1;
	}

	cout << "\n\t\t\t\t\t\t\t\tDo you want to give access to : \n";

	cout << "\n\t\t\t\t\t\t\t\tShow Client List? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pListClients;
	}


	cout << "\n\t\t\t\t\t\t\t\tAdd New Client? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pAddNewClient;
	}


	cout << "\n\t\t\t\t\t\t\t\tDelete Client? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pDeleteClient;
	}


	cout << "\n\t\t\t\t\t\t\t\tUpdate Client? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pUpdateClients;
	}


	cout << "\n\t\t\t\t\t\t\t\tFind Client? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pFindClient;
	}


	cout << "\n\t\t\t\t\t\t\t\tTransactions? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pTranactions;
	}


	cout << "\n\t\t\t\t\t\t\t\tManage Users? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pManageUsers;
	}


	cout << "\n\t\t\t\t\t\t\t\tATM System? y/n? ";
	cin >> Yes_No;

	if (tolower(Yes_No) == 'y')
	{
		Permissions += enMainMenuPermissions::pATMSystem;
	}

	return Permissions;
}

strClients ChangeClientRecord(string AccountNumber)
{
	strClients Clients;

	Clients.AccountNumber = AccountNumber;

	cout << "\n\n\t\t\t\t\t\t\t\tEnter PinCode? ";
	getline(cin >> ws, Clients.PinCode);

	cout << "\n\t\t\t\t\t\t\t\tEnter Name? ";
	getline(cin, Clients.Name);

	cout << "\n\t\t\t\t\t\t\t\tEnter Number Phone? ";
	getline(cin, Clients.NumberPhone);

	cout << "\n\t\t\t\t\t\t\t\tEnter Account Balance? ";
	cin >> Clients.AcountBalance;

	return Clients;
}

strUsers ChangeUserRecord(string Username)
{
	strUsers Users;
	char Yes_No = ' ';

	Users.UserName = Username;

	cout << "\n\t\t\t\t\t\t\t\tEnter Password? ";
	getline(cin >> ws, Users.Password);

	Users.Permissions = ReadPermissionsToSet();

	return Users;
}

string ReadClientAccountNumber()
{
	string Word = "";
	cout << "\n\t\t\t\t\t\t\t\tPlease enter Account Number? ";
	getline(cin >> ws, Word);

	return Word;
}

bool UpdateClientByAccountNumber(vector <strClients>& vClients, string AccountNumber)
{
	strClients Clients;
	char YesOrNo = ' ';

	if (FindClientByAccountNumber(vClients, Clients, AccountNumber))
	{
		PrintClientCard(Clients);

		cout << "\n\t\t\t\t\t\t\t\tAre you sure you want to update this client? ";
		cin >> YesOrNo;

		if (tolower(YesOrNo) == 'y')
		{
			for (strClients& ClientData : vClients)
			{
				if (ClientData.AccountNumber == AccountNumber)
				{
					ClientData = ChangeClientRecord(AccountNumber);
					break;
				}

			}

			SaveCleintsDataToFile(ClientsFileName, vClients);

			ChangeColour(10);
			cout << "\n\t\t\t\t\t\t\t\tClient data has been deleted successfuly.\n ";
			ChangeColour(7);

			return true;
		}
	}

	else
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << AccountNumber << "]";
		ChangeColour(4);
		cout << " is not found !\n";
		ChangeColour(7);
		return false;
	}

}

bool ClientExistsByAccountNumber(string FileName, string AccountNumber)
{
	vector <strClients> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string PrintLine;
		strClients Client;

		while (getline(MyFile, PrintLine))
		{
			Client = ConvertLineClientToRecord(PrintLine);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();

	}

	return false;
}

bool UserExistsByUserName(string FileName, string UserName)
{
	vector <strUsers> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string PrintLine;
		strUsers User;

		while (getline(MyFile, PrintLine))
		{
			User = ConvertLineUsersToRecord(PrintLine);
			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
			vUser.push_back(User);
		}
		MyFile.close();

	}

	return false;
}

strClients ReadNewClient()
{
	strClients ClientData;

	cout << "\n\t\t\t\t\t\t\t\tEnter Account Number? ";
	getline(cin >> ws, ClientData.AccountNumber);

	while (ClientExistsByAccountNumber(ClientsFileName, ClientData.AccountNumber))
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << ClientData.AccountNumber << "]";
		ChangeColour(4);
		cout << " already exists ! ";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tPlease enter another account number . ";
		getline(cin >> ws, ClientData.AccountNumber);
	}

	cout << "\n\t\t\t\t\t\t\t\tEnter PIN Code? ";
	getline(cin, ClientData.PinCode);

	cout << "\n\t\t\t\t\t\t\t\tEnter Name? ";
	getline(cin, ClientData.Name);

	cout << "\n\t\t\t\t\t\t\t\tEnter Phone? ";
	getline(cin, ClientData.NumberPhone);

	cout << "\n\t\t\t\t\t\t\t\tEnter Account Balance? ";
	cin >> ClientData.AcountBalance;

	return ClientData;

}

strUsers ReadNewUser()
{
	strUsers UserData;
	char Yes_No = ' ';


	UserData.UserName = ReadUserName();

	while (UserExistsByUserName(UsersFileName, UserData.UserName))
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << UserData.UserName << "]";
		ChangeColour(4);
		cout << " already exists ! ";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tPlease enter another username . ";
		getline(cin >> ws, UserData.UserName);
	}

	cout << "\n\t\t\t\t\t\t\t\tEnter Password? ";
	getline(cin, UserData.Password);

	UserData.SecurityCode = GetNewSecurityCode();

	cout << "\n\t\t\t\t\t\t\t\tYour Security Code is : " << UserData.SecurityCode;

	UserData.Permissions = ReadPermissionsToSet();

	return UserData;
}

void AddDataLineToFile(string FileName, string DataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;
	}

	MyFile.close();

}

void AddNewClient()
{
	strClients ClientData = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordClientToLine(ClientData));
}

void AddNewClients()
{
	char AddMore = ' ';

	do
	{
		//cout << "\t\t\t\t\t\t\t\tAdding New Client: \n\n";
		AddNewClient();

		ChangeColour(10);
		cout << "\n\t\t\t\t\t\t\t\tThe client has been successfully added";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tDo you want to add a new client ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(vector <strClients>& vClients, string AccountNumber)
{
	strClients Clients;
	char YesOrNo = ' ';

	if (FindClientByAccountNumber(vClients, Clients, AccountNumber))
	{
		PrintClientCard(Clients);

		ChangeColour(4);
		cout << "\n\n\t\t\t\t\t\t\t\tAre you sure you want to delete this client? ";
		cin >> YesOrNo;
		ChangeColour(7);

		if (tolower(YesOrNo) == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(ClientsFileName, vClients);
			vClients = LoadCleintsDataFromFile(ClientsFileName);

			ChangeColour(10);
			cout << "\n\n\t\t\t\t\t\t\t\tClient data has been deleted successfuly.\n ";
			ChangeColour(7);

			return true;
		}

	}

	else
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << AccountNumber << "]";
		ChangeColour(4);
		cout << " is not found !\n";
		ChangeColour(7);

		return false;
	}

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <strClients>& vClients)
{
	char Answer = ' ';

	cout << "\n\t\t\t\t\t\t\t\tAre you sure you want perform this transaction? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (strClients& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AcountBalance += Amount;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				ChangeColour(10);
				cout << "\n\t\t\t\t\t\t\t\tDone successfully !";
				ChangeColour(7);
				cout << " New Balance is: " << C.AcountBalance;

				return true;
			}

		}

		return false;
	}


}

void ShowDepositScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * DEPOSIT SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector<strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	strClients Clients;
	short Amount = 0;

	while (!(FindClientByAccountNumber(vClients, Clients, AccountNumber)))
	{


		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << AccountNumber << "]";
		ChangeColour(4);
		cout << " does not exist ! ";
		ChangeColour(7);

		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Clients);

	cout << "\n\t\t\t\t\t\t\t\tPleas enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * WITHDRAW SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector<strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	strClients Clients;
	short Amount = 0;

	while (!(FindClientByAccountNumber(vClients, Clients, AccountNumber)))
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << AccountNumber << "]";
		ChangeColour(4);
		cout << " does not exist ! ";
		ChangeColour(7);

		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Clients);

	cout << "\n\t\t\t\t\t\t\t\tPleas enter withdraw amount? ";
	cin >> Amount;

	while (Clients.AcountBalance < Amount)
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tAmount Exceeds the balance";
		ChangeColour(7);
		cout << " you can withdraw up to " << Clients.AcountBalance;
		cout << "\n\n\t\t\t\t\t\t\t\tPlease enter another amount: ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalance()
{
	vector <strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	double TotalBalances = 0;

	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t   Balances List [" << vClients.size() << "] Clients(s).\n";
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t| " << left << setw(15) << "Accout Number";
	cout << "\t\t| " << left << setw(40) << "Client Name";
	cout << "\t\t| " << left << setw(12) << "Balance\n";
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n\n";
	ChangeColour(7);

	for (strClients& ClientData : vClients)
	{
		PrintClientRecordBalanceLine(ClientData);
		TotalBalances += ClientData.AcountBalance;
		cout << endl << endl;
	}
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n\n";
	ChangeColour(7);

	cout << "\t\t\t\t\t\t\t\t\t\t Total Balance = " << TotalBalances << "";
}

void PerformTransactionsMenuOption(enTransactionsMenuOption TransactionsMenuOption)
{
	switch (TransactionsMenuOption)
	{
	case enTransactionsMenuOption::Deposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionScreen();
		break;
	}

	case enTransactionsMenuOption::Withdraw:
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionScreen();
		break;
	}

	case enTransactionsMenuOption::TotalBalances:
	{
		system("cls");
		ShowTotalBalance();
		GoBackToTransactionScreen();
		break;
	}

	case enTransactionsMenuOption::MainMenu:
	{
		ShowMainMenu();
		break;
	}

	}

}

void ShowAccessDeniedMessage()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n\n";
	ChangeColour(4);
	cout << "\t\t\t\t\t\t\t\t\t  Access Denied :-( ! \n\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t  You dont have permission to do this.\n\n";
	cout << "\t\t\t\t\t\t\t\t\t  Please Contact Your Admin.\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";

	ChangeColour(7);
}

bool CheckAccessPermission(enMainMenuPermissions Permission)
{
	if (CurrentUser.Permissions == enMainMenuPermissions::eAll)
		return true;


	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;

	else
		return false;

}

void ShowAllClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}


	vector <strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t\t Clients List [" << vClients.size() << "]\n";
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t| " << left << setw(15) << "Accout Number";
	cout << "\t\t| " << left << setw(10) << "Pin Code";
	cout << "\t\t| " << left << setw(40) << "Client Name";
	cout << "\t\t| " << left << setw(12) << "Phone";
	cout << "\t\t| " << left << setw(12) << "Balance\n";
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);


	for (strClients& ClientData : vClients)
	{
		PrintClientRecord(ClientData);
		cout << endl << endl;
	}
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n\n";
	ChangeColour(7);


}

void ShowAddNewClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * ADD NEW CLIENT SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	AddNewClients();
}

void ShowDeleteClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	ChangeColour(4);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * DELETE CLIENT SCREEN * * *\n";
	ChangeColour(4);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector <strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(vClients, AccountNumber);
}

void ShowUpdateClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}


	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * UPDATE CLIENT SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector <strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(vClients, AccountNumber);
}

void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}


	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * FIND CLIENT SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector <strClients> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	strClients Clients;

	if (FindClientByAccountNumber(vClients, Clients, AccountNumber))
	{
		PrintClientCard(Clients);
	}

	else
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tClients with";
		ChangeColour(7);
		cout << " [" << AccountNumber << "]";
		ChangeColour(4);
		cout << " is not found !\n";
		ChangeColour(7);
	}
}

void ShowTransactionsMenuScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	system("cls");
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t       * * * TRANSACTION Menu * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t[1] Deposit.\n";
	cout << "\n\t\t\t\t\t\t\t\t[2] Withdraw.\n";
	cout << "\n\t\t\t\t\t\t\t\t[3] Total Balances.\n";
	cout << "\n\t\t\t\t\t\t\t\t[4] Main Menu.\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	PerformTransactionsMenuOption((enTransactionsMenuOption)ReadTransactionsMenuOption());
}

void ShowManageUsersScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}



	system("cls");
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t       * * * MANAGE USER Menu * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t[1] List Users.\n";
	cout << "\n\t\t\t\t\t\t\t\t[2] Add New User.\n";
	cout << "\n\t\t\t\t\t\t\t\t[3] Delete User.\n";
	cout << "\n\t\t\t\t\t\t\t\t[4] Update User.\n";
	cout << "\n\t\t\t\t\t\t\t\t[5] Find User.\n";
	cout << "\n\t\t\t\t\t\t\t\t[6] Main Menu.\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	PerformManageUsersMenuOption((enManageUsersMenuOption)ReadManageUsersMenuOption());
}

void ShowListUsersScreen()
{
	vector<strUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t\t Users List [" << vUsers.size() << "]\n";
	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);
	cout << "\t\t| " << left << setw(15) << "User Name";
	cout << "\t\t| " << left << setw(10) << "Password";
	cout << "\t\t| " << left << setw(15) << "Permissions\n";


	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n";
	ChangeColour(7);

	if (vUsers.size() == 0)
		cout << "\t\t\t\tNo Users Available In the System !";

	else

		for (strUsers& User : vUsers)
		{
			PrintUersRecord(User);
			cout << endl << endl;
		}

	ChangeColour(1);
	cout << "\t\t_________________________________________________________________________";
	cout << "___________________________________________________________________________\n\n";
	ChangeColour(7);
}

void AddNewUser()
{
	strUsers UserData = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertRecordUserToLine(UserData));
}

void AddNewUsers()
{
	char AddMore = ' ';

	do
	{
		//cout << "Adding New User: \n\n";
		AddNewUser();

		ChangeColour(10);
		cout << "\n\t\t\t\t\t\t\t\tThe user has been successfully added";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tDo you want to add a new user ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewUsersScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * ADD NEW USER SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	AddNewUsers();
}

bool DeleteUserByUsername(vector<strUsers>& vUsers, string Username)
{
	strUsers Users;
	char YesOrNo = ' ';

	if (Username == "Admin")
	{
		ChangeColour(4);
		cout << "\a";
		cout << "\n\n\t\t\t\t\t\t\t\tYou cannot Delete This User.\n";
		ChangeColour(7);
		return true;
	}

	if (FindUserByUsername(vUsers, Users, Username))
	{
		PrintUserCard(Users);

		ChangeColour(4);
		cout << "\n\n\t\t\t\t\t\t\t\tAre you sure you want to delete this user? ";
		cin >> YesOrNo;
		ChangeColour(7);

		if (tolower(YesOrNo) == 'y')
		{
			MarkUserForDeleteByUsername(Username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);
			vUsers = LoadUsersDataFromFile(UsersFileName);

			ChangeColour(10);
			cout << "\n\t\t\t\t\t\t\t\tUser data has been deleted successfuly.\n ";
			ChangeColour(7);

			return true;
		}

	}

	else
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tSorry, The user with username";
		ChangeColour(7);
		cout << " (" << Username << ")";
		ChangeColour(4);
		cout << " does not exist ! ";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tPlease check the username, and try again.\n";

		return false;
	}
}

void ShowDeleteUserScreen()
{
	ChangeColour(4);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * DELETE USER SCREEN * * *\n";
	ChangeColour(4);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector<strUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
	string UserName = ReadUserName();

	DeleteUserByUsername(vUsers, UserName);
}

bool UpdateUserByUsername(vector<strUsers>& vUsers, string Username)
{
	strUsers Users;
	char YesOrNo = ' ';

	if (FindUserByUsername(vUsers, Users, Username))
	{
		PrintUserCard(Users);

		cout << "\n\n\t\t\t\t\t\t\t\tAre you sure you want to update this user? ";
		cin >> YesOrNo;

		if (tolower(YesOrNo) == 'y')
		{
			for (strUsers& UserDate : vUsers)
			{
				if (UserDate.UserName == Username)
				{
					UserDate = ChangeUserRecord(Username);
					break;
				}

			}

			SaveUsersDataToFile(UsersFileName, vUsers);

			ChangeColour(10);
			cout << "\n\t\t\t\t\t\t\t\tUser data has been updated successfuly.\n ";
			ChangeColour(7);

			return true;
		}
	}

	else
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tSorry, The user with username";
		ChangeColour(7);
		cout << " (" << Username << ")";
		ChangeColour(4);
		cout << " does not exist ! ";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tPlease check the username, and try again.\n";
		return false;
	}

}

void ShowUpdateUserScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * UPDATE USER SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector<strUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
	string UserName = ReadUserName();

	UpdateUserByUsername(vUsers, UserName);
}

void ShowFindUserScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t    * * * FIND USER SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	vector<strUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
	string UserName = ReadUserName();
	strUsers Users;

	if (FindUserByUsername(vUsers, Users, UserName))
	{
		PrintUserCard(Users);
	}

	else
	{
		ChangeColour(4);
		cout << "\n\t\t\t\t\t\t\t\tSorry, The user with username";
		ChangeColour(7);
		cout << " (" << UserName << ")";
		ChangeColour(4);
		cout << " does not exist ! ";
		ChangeColour(7);
		cout << "\n\n\t\t\t\t\t\t\t\tPlease check the username, and try again.\n";
	}
}

short getQuickWithDrawAmount(short QuickWithDrawOption)
{
	switch (QuickWithDrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
	if (QuickWithDrawOption == 9)
	{
		return;
	}

	short Amount = getQuickWithDrawAmount(QuickWithDrawOption);

	if (Amount > CurrentClient.AcountBalance)
	{
		AmountExceeds();
		system("cls");
		ShowQuickWithdrawScreen();
		return;
	}

	vector<strClients> vClient = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClient);
	CurrentClient.AcountBalance -= Amount;
}

void ShowQuickWithdrawScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t       * * * QUICK WITHDRAW * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t[1] 20\t\t[2] 50\n";
	cout << "\n\t\t\t\t\t\t\t\t\t[3] 100\t\t[4] 200\n";
	cout << "\n\t\t\t\t\t\t\t\t\t[5] 400\t\t[6] 600\n";
	cout << "\n\t\t\t\t\t\t\t\t\t[7] 800\t\t[8] 1000\n";
	cout << "\n\t\t\t\t\t\t\t\t\t[9] Exit\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\n\t\t\t\t\t\t\t\tYour Balance is " << CurrentClient.AcountBalance << endl;

	PerfromQuickWithdrawOption(ReadQuickWithdrawChoice());
}

int ReadWithdrawAmount()
{
	int Amount = 0;
	cout << "\n\n\t\t\t\t\t\t\t\tEnter an amount Multiple of 5's ? ";
	cin >> Amount;

	while (Amount % 5 != 0)
	{
		cout << "\n\t\t\t\t\t\t\t\tEnter an amount Multiple of 5's ? ";
		cin >> Amount;
	}

	return Amount;
}

void PerfromNormalWithdrawOption()
{
	int Amount = ReadWithdrawAmount();

	if (Amount > CurrentClient.AcountBalance)
	{
		AmountExceeds();
		PerfromNormalWithdrawOption();
		return;
	}

	vector<strClients> vClient = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClient);
	CurrentClient.AcountBalance -= Amount;
}

void ShowNormalWithdrawScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t       * * * NORMAL WITHDRAW * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	PerfromNormalWithdrawOption();
}

void ShowCheckBalanceScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t     * * * CHECK BALANCE SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\n\t\t\t\t\t\t\t\tYour Balance is " << CurrentClient.AcountBalance << endl;

}

double ReadDepositAmount()
{
	double Amount = 0;

	cout << "\n\n\t\t\t\t\t\t\t\tEnter a positive Deposit Amount? ";
	cin >> Amount;

	while (Amount <= 0)
	{
		cout << "\n\n\t\t\t\t\t\t\t\tEnter a positive Deposit Amount? ";
		cin >> Amount;
	}

	return Amount;
}

void PerfromDepositOption()
{
	double Amount = ReadDepositAmount();

	vector<strClients> vClient = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount, vClient);
	CurrentClient.AcountBalance += Amount;
}

void ShowATMDepositScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t       * * * DEPOSIT SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	PerfromDepositOption();
}

string ReadOldPinCode()
{
	string OldPinCode = "";
	cout << "\n\n\t\t\t\t\t\t\t\tPlease enter the Old PinCode: ";
	cin >> OldPinCode;

	while (CurrentClient.PinCode != OldPinCode)
	{
		ChangeColour(4);
		cout << "\a";
		cout << "\n\n\t\t\t\t\t\t\t\tPinCode is wrong ! ";
		ChangeColour(7);
		cout << "Please enter the old pincode : ";
		cin >> OldPinCode;
	}

	return OldPinCode;
}

string ReadNewPinCode()
{
	string NewPinCode = "";
	cout << "\n\n\t\t\t\t\t\t\t\tPlease enter the New PinCode: ";
	cin >> NewPinCode;

	return NewPinCode;
}

void PerfromChangePinCodeOption()
{
	string OldPinCode = ReadOldPinCode();

	string NewPinCode = ReadNewPinCode();

	while (OldPinCode == NewPinCode)
	{
		ChangeColour(4);
		cout << "\a";
		cout << "\n\n\t\t\t\t\t\t\t\tThis is the same old PinCode ! ";
		cout << "Please enter another one : ";
		cin >> NewPinCode;
	}

	vector<strClients> vClient = LoadCleintsDataFromFile(ClientsFileName);
	ChangePinCodeToFileByOldPinCode(OldPinCode, NewPinCode, vClient);
	CurrentClient.PinCode = NewPinCode;

}

void ShowChangePinCodeScreen()
{
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t       * * * CHANGE PINCODE * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	PerfromChangePinCodeOption();
}

void PerformATMMainMenuOption(enATMMainMenuOption ATMMainMenuOption)
{
	switch (ATMMainMenuOption)
	{
	case enATMMainMenuOption::QuickWithdraw:
	{
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToATMMainScreen();
		break;
	}

	case enATMMainMenuOption::NormalWithdraw:
	{
		system("cls");
		ShowNormalWithdrawScreen();
		GoBackToATMMainScreen();
		break;
	}

	case enATMMainMenuOption::ATMDeposit:
	{
		system("cls");
		ShowATMDepositScreen();
		GoBackToATMMainScreen();
		break;
	}

	case enATMMainMenuOption::ATMTotalBalances:
	{
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToATMMainScreen();
		break;
	}

	case enATMMainMenuOption::ChangePinCode:
	{
		system("cls");
		ShowChangePinCodeScreen();
		GoBackToATMMainScreen();
		break;
	}


	case enATMMainMenuOption::Mainn:
	{
		system("cls");
		ShowMainMenu();
		break;
	}

	}

}

void ShowATMMainMenuScreen()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pATMSystem))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}


	system("cls");
	system("cls");
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t  * * * ATM MAIN Menu SCREEN * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t[1] Quick Withdraw.\n";
	cout << "\n\t\t\t\t\t\t\t\t[2] Normal Withdraw.\n";
	cout << "\n\t\t\t\t\t\t\t\t[3] Deposit.\n";
	cout << "\n\t\t\t\t\t\t\t\t[4] Check Balance.\n";
	cout << "\n\t\t\t\t\t\t\t\t[5] Change PinCode.\n";
	cout << "\n\t\t\t\t\t\t\t\t[6] Main Menu.\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
	cout << "\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	PerformATMMainMenuOption((enATMMainMenuOption)ReadATMMainMenuOption());
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, strClients& Client)
{
	vector<strClients> vClient = LoadCleintsDataFromFile(ClientsFileName);

	for (strClients& C : vClient)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;

	else
		return false;
}

void LoginATMSystem()
{
	bool LoginFaild = false;
	string AccountNumber = "", PinCode = "";

	do
	{
		system("cls");
		ChangeColour(9);
		cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
		cout << "\t\t\t\t\t\t\t\t______________________________\n";
		ChangeColour(7);
		cout << "\t\t\t\t\t\t\t\t\t * * * LOGIN TO ATM Menu SCREEN * * *\n";
		ChangeColour(9);
		cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
		cout << "\t\t\t\t\t\t\t\t______________________________\n";
		ChangeColour(7);

		if (LoginFaild)
		{
			ChangeColour(4);
			cout << "\n\t\t\t\t\t\t\t\tIncorrect account number or pincode :-( ! ";
			ChangeColour(7);
			cout << "Please re-enter.\n";
		}

		AccountNumber = ReadClientAccountNumber();
		PinCode = ReadPinCode();

		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFaild);

	ShowATMMainMenuScreen();
}

void PerformManageUsersMenuOption(enManageUsersMenuOption ManageUsers)
{
	switch (ManageUsers)
	{
	case enManageUsersMenuOption::ListUsers:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUsersScreen();
		break;
	}

	case enManageUsersMenuOption::AddUsers:
	{
		system("cls");
		ShowAddNewUsersScreen();
		GoBackToManageUsersScreen();
		break;
	}

	case enManageUsersMenuOption::DeleteUesrs:
	{
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersScreen();
		break;
	}

	case enManageUsersMenuOption::UpdateUsers:
	{
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersScreen();
		break;
	}

	case enManageUsersMenuOption::FindUsers:
	{
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersScreen();
		break;
	}

	case enManageUsersMenuOption::Main:
	{
		ShowMainMenu();
		break;
	}

	}

}

void PerformMainMenuOption(enMainMenuOption MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOption::Show:
	{
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainScreen();
		break;
	}

	case enMainMenuOption::Add:
	{
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainScreen();
		break;
	}

	case enMainMenuOption::Delete:
	{
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainScreen();
		break;
	}

	case enMainMenuOption::Update:
	{
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainScreen();
		break;
	}

	case enMainMenuOption::Find:
	{
		system("cls");
		ShowFindClientScreen();
		GoBackToMainScreen();
		break;
	}

	case enMainMenuOption::Transactions:
	{
		system("cls");
		ShowTransactionsMenuScreen();
		break;
	}

	case enMainMenuOption::ManageUsers:
	{
		system("cls");
		ShowManageUsersScreen();
		break;
	}

	case enMainMenuOption::ATMSystem:
	{
		system("cls");
		LoginATMSystem();
		break;
	}

	case enMainMenuOption::Logout:
	{
		system("cls");
		Login();
		break;
	}

	}

}

void ShowMainMenu()
{
	system("cls");
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t\t\t  * * * MAIN Menu * * *\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);
	cout << "\t\t\t\t\t\t\t\t[1] Show Client List.\n";
	cout << "\n\t\t\t\t\t\t\t\t[2] Add New Client.\n";
	cout << "\n\t\t\t\t\t\t\t\t[3] Delete Client.\n";
	cout << "\n\t\t\t\t\t\t\t\t[4] Update Client Info.\n";
	cout << "\n\t\t\t\t\t\t\t\t[5] Find Client.\n";
	cout << "\n\t\t\t\t\t\t\t\t[6] Transactions.\n";
	cout << "\n\t\t\t\t\t\t\t\t[7] Manage Users.\n";
	cout << "\n\t\t\t\t\t\t\t\t[8] ATM System.\n";
	cout << "\n\t\t\t\t\t\t\t\t[9] Logout.\n";
	ChangeColour(9);
	cout << "\t\t\t\t\t\t\t\t______________________________________________________\n\t\t\t\t\t\t\t\t______________________________\n";
	ChangeColour(7);

	PerformMainMenuOption((enMainMenuOption)ReadMainMenuOption());
}

bool FindUserByUsernameAndPassword(string Username, string Password, strUsers& User)
{
	vector<strUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

	for (strUsers& U : vUsers)
	{
		/*if (U.UserName == Username)
			CurrentUser.UserName = Username;*/

		if ((U.UserName == Username) && (U.Password == Password))
		{
			User = U;
			return  true;
		}
	}

	return false;
}

bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;

	else
		return false;
}

string ReadSecurityCode()
{
	string SecurityCode = "";
	cout << "\n\t\t\t\t\t\t\t\tPlease enter the Security Code: ";
	cin >> SecurityCode;

	while (CurrentUser.SecurityCode != SecurityCode)
	{
		ChangeColour(4);
		cout << "\a";
		cout << "\n\t\t\t\t\t\t\t\tSecurity Code is wrong !";
		ChangeColour(7);
		cout << " Please enter the correct Security Code : ";
		cin >> SecurityCode;
	}

	ChangeColour(10);
	cout << "\n\t\t\t\t\t\t\t\tSecurity Code is correct :-)\n";
	ChangeColour(7);


	return SecurityCode;
}

string ReadNewPassword()
{
	string NewPassword = "";
	cout << "\n\t\t\t\t\t\t\t\tPlease enter the New Password: ";
	cin >> NewPassword;

	return NewPassword;
}

void PerfromChangePassword()
{
	vector<strUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

	char Answer = ' ';
	string Username = "";
	bool UsernameFail = true;
	cout << "\n\t\t\t\t\t\t\t\tDo you want to change your password? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{

		while (UsernameFail)
		{
			Username = ReadUserName();

			for (strUsers& U : vUsers)
			{

				if ((U.UserName == Username))
				{
					CurrentUser = U;
					UsernameFail = false;
					break;
				}
			}

			if (UsernameFail)
			{
				ChangeColour(4);
				cout << "\n\t\t\t\t\t\t\t\tUsername does not exist ! \n";
				ChangeColour(7);
				/*cout << " Please enter your username : ";
				getline(cin >> ws, Username);*/
			}

		}


	}

	else
	{
		return;
	}

	string SecurityCode = ReadSecurityCode();

	string NewPassword = ReadNewPassword();

	while (CurrentUser.Password == NewPassword)
	{
		ChangeColour(4);
		cout << "\a";
		cout << "\n\t\t\t\t\t\t\t\tThis is the same old Password ! ";
		ChangeColour(7);
		cout << "Please enter another one : ";
		cin >> NewPassword;
	}

	ChangePasswordToFileBySecurityCode(SecurityCode, NewPassword, vUsers);
	CurrentUser.Password = NewPassword;
}

void Login()
{
	bool LoginFaild = false;
	bool ChangePassword = false;
	string Username, Password;
	char Answer = ' ';
	short Counter = 0;
	do
	{
		Counter++;
		system("cls");
		ChangeColour(9);
		cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
		cout << "\t\t\t\t\t\t\t\t______________________________\n";
		ChangeColour(7);
		cout << "\t\t\t\t\t\t\t\t\t\t  * * * LOGIN SCREEN * * *\n";
		ChangeColour(9);
		cout << "\t\t\t\t\t\t\t\t______________________________________________________\n";
		cout << "\t\t\t\t\t\t\t\t______________________________\n";
		ChangeColour(7);

		/*if (ChangePassword)
		{
			PerfromChangePassword();
			break;
		}*/

		if (LoginFaild)
		{
			ChangeColour(4);
			cout << "\n\t\t\t\t\t\t\t\tIncorrect username or password :-( ! ";
			ChangeColour(7);
			cout << "Please re-enter.\n";
		}

		Username = ReadUserName();
		Password = ReadPassword();

		LoginFaild = !LoadUserInfo(Username, Password);

		if (Counter >= 3)
		{
			ChangeColour(4);
			cout << "\n\t\t\t\t\t\t\t\tThe Password is wrong,";
			ChangeColour(1);
			cout << " Forgotten password ? y/n ? ";
			cin >> Answer;
			ChangeColour(7);

			if (tolower(Answer) == 'y')
			{
				PerfromChangePassword();
				Login();
				return;
			}
		}

	} while (LoginFaild);

	ShowMainMenu();


}



int main()
{
	srand((unsigned)time(NULL));

	Login();



	system("pause>0");

	return 0;

}