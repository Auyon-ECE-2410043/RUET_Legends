#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
using namespace std;

// ---------- SAFE INPUT ----------
int safeInt(const string &prompt = "")
{
    int x;
    while (true)
    {
        try
        {
            if (!prompt.empty())
                cout << prompt;
            cin >> x;

            if (cin.fail())
                throw runtime_error("Invalid input!");

            if (x <= 0)
                throw runtime_error("Must be positive!");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        catch (runtime_error &e)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << e.what() << " Try again.\n";
        }
    }
}

float safeFloat(const string &prompt = "")
{
    float x;
    while (true)
    {
        try
        {
            if (!prompt.empty())
                cout << prompt;
            cin >> x;

            if (cin.fail())
                throw runtime_error("Invalid input!");

            if (x < 0)
                throw runtime_error("Cannot be negative!");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        catch (runtime_error &e)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << e.what() << " Try again.\n";
        }
    }
}

// ---------- TEMPLATE ----------
template <typename T>
void displayVector(const vector<T> &v)
{
    for (const auto &item : v)
        item.displayInfo();
}

// ---------- ROOM ----------
class Room
{
private:
    int roomNo;
    string type;
    float price;
    bool isBooked;

public:
    Room() : roomNo(0), type(""), price(0), isBooked(false) {}
    Room(int r, string t, float p) : roomNo(r), type(t), price(p), isBooked(false) {}

    int getRoomNo() const { return roomNo; }
    float getPrice() const { return price; }
    bool getStatus() const { return isBooked; }

    void book() { isBooked = true; }
    void free() { isBooked = false; }

    void displayInfo() const
    {
        cout << left << setw(10) << roomNo
             << setw(10) << type
             << setw(10) << fixed << setprecision(2) << price
             << setw(10) << (isBooked ? "Booked" : "Free") << endl;
    }
};
class Person
{
protected:
    string name;

public:
    Person()
    {
        name = "";
    }
    Person(string n)
    {
        name = n;
    }
    virtual void displayInfo() const = 0;
    string getName() const
    {
        return name;
    }
};
class Customer : public Person
{
private:
    int custID;
    int roomNo;
    int days;

public:
    static int totalCustomers;
    Customer() : Person()
    {
        custID = 0;
        roomNo = 0;
        days = 0;
    }
    Customer(int id, string n, int r, int d) : Person(n)
    {
        custID = id;
        roomNo = r;
        days = d;
        totalCustomers++;
    }
    Customer(const Customer &c) : Person(c.name)
    {
        custID = c.custID;
        roomNo = c.roomNo;
        days = c.days;
        totalCustomers++;
    }
    ~Customer()
    {
        totalCustomers--;
    }
    int getRoomNo() const
    {
        return roomNo;
    }
    int getDays() const
    {
        return days;
    }
    int getCustID() const
    {
        return custID;
    }
    friend class Bill;
    void displayInfo() const override
    {
        cout << custID << " | "
             << name << " | Room: "
             << roomNo << " | Days: "
             << days << endl;
    }
};
int Customer::totalCustomers = 0;
class Bill
{
public:
    float roomBill;
    float foodBill;
    float totalBill;

    Bill()
    {
        roomBill = 0;
        foodBill = 0;
        totalBill = 0;
    }
    void calculate(Customer &c, float price, float food)
    {
        roomBill = price * c.getDays();
        foodBill = food;
        totalBill = roomBill + foodBill;
    }
};
class AdminBase : public Person
{
protected:
    string username;
    string password;

public:
    AdminBase() : Person()
    {
        username = "admin";
        password = "1234";
    }

    virtual bool login(string u, string p)
    {
        if (u == username && p == password)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void displayInfo() const override
    {
        cout << "Admin: " << username << endl;
    }
    bool authenticate()
    {
        string user, pass;
        int attempts = 3;

        while (attempts--)
        {
            cout << "Enter Username: ";
            cin >> user;
            cout << "Enter Password: ";
            cin >> pass;

            if (login(user, pass))
                return true;
            cout << "Wrong credentials! Attempts left: " << attempts << endl;
        }
        return false;
    }
};
// ---------- MAIN ----------
int main()
{
    AdminBase admin;

    if (!admin.authenticate())
        return 0;

    Hotel h;
    h.run();
}