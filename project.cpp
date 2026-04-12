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