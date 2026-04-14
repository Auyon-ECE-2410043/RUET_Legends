#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
using namespace std;

// ============================================================
//   DIAMOND INHERITANCE STRUCTURE
//
//              Person
//             /      \
//       AdminBase   StaffMember
//             \      /
//             Manager
//
//  A Manager IS-A AdminBase (has login, manages hotel)
//  A Manager IS-A StaffMember (works shifts, has salary)
//  Both AdminBase and StaffMember share ONE Person (virtual)
// ============================================================
// Error Handling
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
                throw runtime_error("Invalid input! Not an integer.");
            if (x <= 0)
                throw runtime_error("Value must be positive!");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        catch (runtime_error &e)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << e.what() << " Enter again.\n";
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
                throw runtime_error("Invalid input! Not a number.");
            if (x < 0)
                throw runtime_error("Value cannot be negative!");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        catch (runtime_error &e)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << e.what() << " Enter again.\n";
        }
    }
}
// Generic Function / Template
template <typename T>
void displayVector(const vector<T> &v)
{
    for (const auto &item : v)
        item.displayInfo();
}

// ─────────────────────────────────────────────
//  BASE: Person
//  Every human in this system has a name.
// ─────────────────────────────────────────────
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
    virtual void displayInfo() const = 0; // Pure Virtual Function
    string getName() const
    {
        return name;
    }
};

// ─────────────────────────────────────────────
//  LEFT BRANCH: AdminBase
//  An admin can log in and manage the hotel.
//  Uses virtual inheritance → shares Person with StaffMember.
// ─────────────────────────────────────────────
class AdminBase : virtual public Person
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
    AdminBase(string n, string u, string p) : Person(n)
    {
        username = u;
        password = p;
    }

    virtual bool login(string u, string p)
    {
        return (u == username && p == password);
    }

    void displayInfo() const override
    {
        cout << "[Admin] Name: " << name
             << " | Username: " << username << endl;
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

//  RIGHT BRANCH: StaffMember
//  A staff member works shifts and earns a salary.
//  Uses virtual inheritance → shares Person with AdminBase.

class StaffMember : virtual public Person
{
protected:
    string role;
    float salary;
    int shiftHours;

public:
    StaffMember() : Person()
    {
        role = "";
        salary = 0;
        shiftHours = 0;
    }
    StaffMember(string n, string r, float s, int sh) : Person(n)
    {
        role = r;
        salary = s;
        shiftHours = sh;
    }

    void displayInfo() const override
    {
        cout << "[Staff] Name: " << name
             << " | Role: " << role
             << " | Salary: " << fixed << setprecision(2) << salary
             << " | Shift: " << shiftHours << "hrs" << endl;
    }

    string getRole() const
    {
        return role;
    }
    float getSalary() const
    {
        return salary;
    }
    int getShift() const
    {
        return shiftHours;
    }

    // Staff can take on extra shifts (overtime)
    void addOvertime(int extraHours)
    {
        shiftHours += extraHours;
        salary += extraHours * 150.0f; // 150 per extra hour
        cout << name << " worked " << extraHours
             << " overtime hours. New salary: " << salary << endl;
    }
};

//  DIAMOND TIP: Manager
//
//  A Manager IS-A AdminBase  → can log in, manage hotel
//  A Manager IS-A StaffMember → has a salary, works shifts
//
//  WITHOUT virtual inheritance:
//    Manager would have TWO copies of Person (name stored twice).
//    manager.getName() → AMBIGUOUS compiler error!
//
//  WITH virtual inheritance (above):
//    Only ONE shared Person exists. getName() works perfectly.
//
//  Real-life justification:
//    In a hotel, the Manager is a staff member who ALSO has
//    admin privileges. They clock in for shifts AND manage
//    bookings. One person — two roles — one name. ✅
// ─────────────────────────────────────────────
class Manager : public AdminBase, public StaffMember
{
private:
    string department;

public:
    Manager(string n, string u, string p,
            string r, float s, int sh, string dept)
        : Person(n), // ← Person constructed ONCE here (virtual base)
          AdminBase(n, u, p),
          StaffMember(n, r, s, sh)
    {
        department = dept;
    }

    // Single, unified displayInfo — no ambiguity
    void displayInfo() const override
    {
        cout << "\n========== Manager Profile ==========\n";
        cout << "  Name       : " << name << endl; // ONE name from ONE Person
        cout << "  Username   : " << username << endl;
        cout << "  Department : " << department << endl;
        cout << "  Role       : " << role << endl;
        cout << "  Salary     : " << fixed << setprecision(2) << salary << endl;
        cout << "  Shift Hours: " << shiftHours << " hrs" << endl;
        cout << "=====================================\n";
    }

    // Manager can approve overtime for themselves
    void approveOwnOvertime(int hours)
    {
        cout << "\n[Manager Action] " << name
             << " approved own overtime:\n";
        addOvertime(hours); // from StaffMember
    }

    string getDepartment() const
    {
        return department;
    }
};

class Room
{
private:
    int roomNo;
    string type;
    float price;
    bool isBooked;

public:
    Room() : roomNo(0), type(""), price(0), isBooked(false)
    {
    }
    Room(int r, string t, float p) : roomNo(r), type(t), price(p), isBooked(false)
    {
    }

    int getRoomNo() const
    {
        return roomNo;
    }
    float getPrice() const
    {
        return price;
    }
    bool getStatus() const
    {
        return isBooked;
    }
    void book()
    {
        isBooked = true;
    }
    void free()
    {
        isBooked = false;
    }

    void displayInfo() const
    {
        cout << left << setw(10) << roomNo
             << setw(10) << type
             << fixed << setprecision(2)
             << setw(10) << price
             << setw(10) << (isBooked ? "Booked" : "Free")
             << endl;
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

    Customer() : Person(), custID(0), roomNo(0), days(0)
    {
    }
    Customer(int id, string n, int r, int d) : Person(n), custID(id), roomNo(r), days(d)
    {
        totalCustomers++;
    }
    Customer(const Customer &c) : Person(c.name), custID(c.custID), roomNo(c.roomNo), days(c.days)
    {
        totalCustomers++;
    }
    ~Customer() { totalCustomers--; }

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

    friend class Bill; // Bill Class is a friend of Customer Class.

    void displayInfo() const override
    {
        cout << custID << " | " << name
             << " | Room: " << roomNo
             << " | Days: " << days << endl;
    }
};
int Customer::totalCustomers = 0;

class Bill
{
public:
    float roomBill, foodBill, totalBill;
    Bill() : roomBill(0), foodBill(0), totalBill(0) {}

    void calculate(Customer &c, float price, float food)
    {
        roomBill = price * c.getDays();
        foodBill = food;
        totalBill = roomBill + foodBill;
    }
};

//  Hotel — uses composition for customers/rooms
//          managed by a Manager (diamond class)

class Hotel
{
private:
    vector<Room> rooms;
    vector<Customer> customers;
    Manager *manager; // Hotel is run by a Manager

public:
    Hotel(Manager *mgr) : manager(mgr)
    {
    }

    void addRoom()
    {
        int no = safeInt("Enter Room No: ");
        for (size_t i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].getRoomNo() == no)
            {
                cout << "Room exists!\n";
                return;
            }
        }
        string type;
        while (true)
        {
            cout << "Type (single/double): ";
            cin >> type;
            if (type == "single" || type == "double" ||
                type == "SINGLE" || type == "DOUBLE")
                break;
            cout << "Invalid type! Only 'single' or 'double' allowed.\n";
        }
        float price = safeFloat("Enter Price: "); // Error Handling
        rooms.push_back(Room(no, type, price));
        cout << "Room added successfully!\n";
    }

    void displayRooms() const
    {
        cout << left << setw(10) << "Room"
             << setw(10) << "Type"
             << setw(10) << "Price"
             << setw(10) << "Status" << endl;
        displayVector(rooms); // Generic Function or Template Function
    }

    void bookRoom()
    {
        int id;
        while (true)
        {
            id = safeInt("Enter Customer ID: "); // Error Handling
            bool exists = false;
            for (const auto &c : customers) // Goes through each element in vector customers, without copying it and it doesn't allow any changes.
                if (c.getCustID() == id)
                {
                    exists = true;
                    break;
                }
            if (exists)
                cout << "Customer ID already exists! Enter a different ID.\n";
            else
                break;
        }

        string name;
        cout << "Enter Name: ";
        getline(cin, name); // It takes full input (including spaces) from the user and stores it in the variable name

        int roomNo = safeInt("Enter Room No: "); // Error Handling for roomNo
        int days = safeInt("Enter Days: ");      // Error Handling for days

        for (size_t i = 0; i < rooms.size(); i++) // Loops through all rooms in the hotel
        {
            if (rooms[i].getRoomNo() == roomNo && !rooms[i].getStatus())
            {
                rooms[i].book();                                       // Marks the room as booked
                customers.push_back(Customer(id, name, roomNo, days)); // Adds new customer to the customers vector
                cout << "Room booked successfully!\n";
                return;
            }
        }
        cout << "Room is already Booked.!\n";
    }

    void checkOut()
    {
        int room = safeInt("Enter Room No: ");
        for (size_t i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].getRoomNo() == room && rooms[i].getStatus())
            {
                for (size_t j = 0; j < customers.size(); j++)
                {
                    if (customers[j].getRoomNo() == room)
                    {
                        int days = customers[j].getDays();
                        float food = 1500 * days;

                        Bill b; // Bill is the friend class of Customer class.
                        b.calculate(customers[j], rooms[i].getPrice(), food);

                        cout << "\n========== BILL ==========\n";
                        cout << "Customer : " << customers[j].getName() << endl;
                        cout << "Room Bill: " << b.roomBill << endl;
                        cout << "Food Bill: " << b.foodBill << endl;
                        cout << "Total    : " << b.totalBill << endl;
                        cout << "Checked out by Manager: "
                             << manager->getName() << endl; // ONE name — no ambiguity

                        customers.erase(customers.begin() + j);
                        rooms[i].free();
                        return;
                    }
                }
            }
        }
        cout << "Invalid Room or Not Booked!\n";
    }

    void displayCustomers() const
    {
        if (customers.empty())
        {
            cout << "No customers currently in the hotel.\n";
            return;
        }
        displayVector(customers); // Calling Generic Function
    }

    void searchRoom() const
    {
        int num = safeInt("Enter Room No: ");
        for (size_t i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].getRoomNo() == num)
            {
                rooms[i].displayInfo();
                return;
            }
        }
        cout << "Room not found!\n";
    }

    int getTotalCustomers() const
    {
        return customers.size();
    }

    void showManagerProfile() const
    {
        manager->displayInfo();
    }

    void managerOvertime()
    {
        int hours = safeInt("Enter overtime hours for manager: ");
        manager->approveOwnOvertime(hours);
    }

    void run()
    {
        int choice;
        cout << "\nWelcome, Manager " << manager->getName()
             << "! (" << manager->getDepartment() << " Dept)\n";

        while (true)
        {
            cout << "\n====== HOTEL MENU ======\n";
            cout << "1. Add Room\n";
            cout << "2. Book Room\n";
            cout << "3. Check Out\n";
            cout << "4. Display Rooms\n";
            cout << "5. Display Customers\n";
            cout << "6. Search Room\n";
            cout << "7. Total Customers\n";
            cout << "8. Manager Profile\n";
            cout << "9. Manager Overtime\n";
            cout << "10. Exit\n";
            cout << "Enter choice: ";
            choice = safeInt(); // Error handling for choice

            switch (choice)
            {
            case 1:
                addRoom();
                break;
            case 2:
                bookRoom();
                break;
            case 3:
                checkOut();
                break;
            case 4:
                displayRooms();
                break;
            case 5:
                displayCustomers();
                break;
            case 6:
                searchRoom();
                break;
            case 7:
                cout << "Total Customers: " << getTotalCustomers() << endl;
                break;
            case 8:
                showManagerProfile();
                break;
            case 9:
                managerOvertime();
                break;
            case 10:
            {
                string confirm;
                cout << "Exit? (YES/NO): ";
                cin >> confirm;
                if (confirm == "yes" || confirm == "YES")
                    return;
                break;
            }
            default:
                cout << "Invalid choice!\n";
            }
        }
    }
};

// ─────────────────────────────────────────────
//  main()
// ─────────────────────────────────────────────
int main()
{
    // Create a Manager — the diamond class
    // ONE Person, TWO roles: Admin + StaffMember
    Manager mgr(
        "Auyon",         // name  (from Person — shared, not duplicated)
        "admin",         // username (from AdminBase)
        "1234",          // password (from AdminBase)
        "Hotel Manager", // role     (from StaffMember)
        85000.0f,        // salary   (from StaffMember)
        8,               // shift hours (from StaffMember)
        "Operations"     // department (Manager's own field)
    );

    cout << "\n===== Hotel Management System =====\n";
    cout << "Manager login required.\n\n";

    // authenticate() comes from AdminBase path
    if (!mgr.authenticate()) // it checks if the manager login is successful or not
    {
        cout << "Login Failed! Access denied.\n";
        return 0;
    }

    cout << "\nLogin Successful!\n";

    // Hotel is managed by our Manager (diamond class)
    Hotel hotel(&mgr);
    hotel.run();

    return 0;
}