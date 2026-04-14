# Hotel Management System

<div align="center">

**A Console-Based Application Implementing Object-Oriented Programming Principles in C++**

Rajshahi University of Engineering & Technology (RUET)
Department of Electrical & Computer Engineering
Course: Object-Oriented Programming (ECE-1204)

---

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue?style=flat-square)
![OOP](https://img.shields.io/badge/Paradigm-Object--Oriented-green?style=flat-square)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=flat-square)
![Diamond](https://img.shields.io/badge/Feature-Diamond%20Inheritance-orange?style=flat-square)

</div>

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [System Description](#2-system-description)
3. [OOP Concepts Applied](#3-oop-concepts-applied)
4. [Class Design & Architecture](#4-class-design--architecture)
5. [Diamond Problem — The Core Design Challenge](#5-diamond-problem--the-core-design-challenge)
6. [Feature Walkthrough](#6-feature-walkthrough)
7. [Real-Life Problems & Solutions](#7-real-life-problems--solutions)
8. [Build & Run Instructions](#8-build--run-instructions)
9. [Limitations & Future Work](#9-limitations--future-work)
10. [Authors](#10-authors)

---

## 1. Project Overview

This project presents the **design and implementation of a Hotel Management System** built entirely in C++ using Object-Oriented Programming (OOP) principles. The system models a real-world hotel environment where a manager — who is simultaneously a staff member and an administrator — can manage rooms, book guests, process checkouts, auto-calculate bills, and track occupancy in real time.

The primary objective of this project is to demonstrate the practical application of core OOP concepts — namely **encapsulation, inheritance, polymorphism, abstraction, and the diamond problem** — within a functional, multi-class C++ application.

The system operates on a **console-based interface** with no graphical dependencies, making it portable across all major platforms that support C++17.

---

## 2. System Description

### 2.1 Hotel Context

The system simulates a real hotel environment with the following operational rules:

| Rule | Details |
|---|---|
| Room types supported | Single / Double |
| Booking method | By Room Number and Customer ID |
| Billing | Auto-calculated (Room Bill + Food Bill) |
| Authentication | Username + Password with 3-attempt lockout |
| Manager role | Admin privileges + Staff identity (Diamond class) |
| Food charge rate | BDT 1500 per day (flat rate) |

### 2.2 System Rules

| Rule | Behaviour |
|---|---|
| Duplicate room numbers | Rejected at entry |
| Duplicate customer IDs | Rejected before booking |
| Invalid input (letters/negatives) | Caught and re-prompted automatically |
| Already booked rooms | Cannot be double-booked |
| Checkout on unbooked room | Rejected with error message |
| Manager overtime | Logged and salary updated instantly |

---

## 3. OOP Concepts Applied

This section maps each OOP concept to the specific class or construct in the project where it is demonstrated.

### 3.1 Encapsulation

Every class enforces strict data hiding. All data members are declared `private` or `protected`. External access is only possible through explicitly defined `public` getter and setter methods.

**Example — Room class:**
```cpp
class Room {
private:
    int    roomNo;    // Cannot be accessed or modified directly
    string type;     // from outside the class
    float  price;
    bool   isBooked;

public:
    int   getRoomNo() const { return roomNo;   }  // Controlled read
    float getPrice()  const { return price;    }  // access only
    void  book()            { isBooked = true; }  // Controlled write
};
```

### 3.2 Inheritance — Multi-Level Hierarchy

The project implements a carefully designed class hierarchy:

```
Person            (virtual base — shared by all)
  ├── AdminBase   (virtual inheritor — login, authentication)
  └── StaffMember (virtual inheritor — salary, shifts, overtime)
        └── Manager  ← Diamond class (IS-A Admin AND Staff)

Person
  └── Customer    (guest who books rooms)
```

### 3.3 Runtime Polymorphism

`Person` declares `displayInfo()` as a pure virtual function. Every derived class — `AdminBase`, `StaffMember`, `Manager`, `Customer` — overrides this method. The correct version is selected at runtime depending on the actual object type.

```cpp
Person* p = new Manager(...);
p->displayInfo();   // Calls Manager::displayInfo() at runtime
delete p;
```

### 3.4 Abstraction

`Person` is an abstract class. It cannot be instantiated and defines a contract that all derived classes must fulfil through the pure virtual `displayInfo()` function.

```cpp
class Person {
public:
    virtual void displayInfo() const = 0;  // Pure virtual — forces derived classes
};                                          // to define their own display behaviour
```

### 3.5 Diamond Problem & Virtual Inheritance

`Manager` inherits from both `AdminBase` and `StaffMember`. Without virtual inheritance, `Manager` would receive two copies of `Person`'s data — causing ambiguity. The `virtual` keyword ensures only one shared copy of `Person` exists inside `Manager`.

> Full explanation in [Section 5](#5-diamond-problem--the-core-design-challenge).

### 3.6 Static Members

Static members belong to the class itself — shared across all instances.

| Class | Static Member | Purpose |
|---|---|---|
| `Customer` | `totalCustomers` | Live count of all customers currently in the hotel |

### 3.7 Friend Class

`Bill` is declared a `friend` of `Customer`. This grants `Bill` direct access to `Customer`'s private members — specifically `days` — to calculate the room bill accurately without exposing those fields publicly.

```cpp
class Customer {
    int days;  // private
    friend class Bill;
};
```

### 3.8 Templates

The `displayVector()` template function works with any type that has a `displayInfo()` method — used to display both rooms and customers from a single reusable function.

```cpp
template <typename T>
void displayVector(const vector<T>& v) {
    for (const auto& item : v)
        item.displayInfo();
}
```

### 3.9 Constructors and Destructors

| Constructor Type | Demonstrated In |
|---|---|
| Default constructor | All classes |
| Parameterised constructor | `Room`, `Customer`, `Manager`, `Bill`, etc. |
| Copy constructor | `Customer` (with static counter increment) |
| Destructor | `Customer` (decrements static counter on checkout) |

---

## 4. Class Design & Architecture

### Class Responsibility Summary

| Class | Type | Responsibility |
|---|---|---|
| `Person` | Abstract | Root base — holds name; defines display contract |
| `AdminBase` | Concrete (virtual) | Handles login, password, authentication |
| `StaffMember` | Concrete (virtual) | Holds salary, shift hours, overtime logic |
| `Manager` | Concrete (Diamond) | Admin + Staff identity; resolves diamond |
| `Customer` | Concrete | Stores guest booking details |
| `Room` | Concrete | Tracks room number, type, price, booking status |
| `Bill` | Concrete | Auto-calculates room and food bill at checkout |
| `Hotel` | Controller | Manages all operations via composition |

### Inheritance Diagram

```
                    Person
                   /      \
            AdminBase    StaffMember
             (login,      (salary,
            password)      shifts)
                   \      /
                   Manager        ← ONE Person (virtual inheritance)
                   (dept, admin + staff)


Person
  └── Customer    (separate branch — guest booking)
```

---

## 5. Diamond Problem — The Core Design Challenge

### The Problem

A `Manager` in a real hotel is **both** an admin and a staff member. Both `AdminBase` and `StaffMember` inherit from `Person`. Without special handling, `Manager` receives **two separate copies** of `Person` — meaning the name is stored twice, and calling `manager.getName()` causes a **compile-time ambiguity error**.

```
           Person          Person
             |                |
         AdminBase        StaffMember
              \              /
               Manager (TWO names — WRONG!)
```

### The Solution — Virtual Inheritance

```cpp
class AdminBase   : virtual public Person { ... };  // share one Person
class StaffMember : virtual public Person { ... };  // share one Person

class Manager : public AdminBase, public StaffMember {
public:
    Manager(string n, ...)
        : Person(n),        // ← Person constructed ONCE here
          AdminBase(...),
          StaffMember(...)
    {}
};
```

### After Fix

```
              Person          ← ONE shared instance
             /      \
       AdminBase   StaffMember
              \      /
              Manager         ← ONE name, TWO roles ✅
```

### Real-Life Justification

> In a hotel, the Manager clocks in for shifts and earns a salary (StaffMember), AND has login access to manage rooms and bookings (AdminBase). He is **one person** — not two. That is exactly why virtual inheritance is not just a technical fix, but a reflection of reality.

---

## 6. Feature Walkthrough

| Menu Option | Function | Description |
|---|---|---|
| 1. Add Room | `addRoom()` | Add a room with number, type, and price — duplicates rejected |
| 2. Book Room | `bookRoom()` | Assign a customer to a free room — duplicate IDs rejected |
| 3. Check Out | `checkOut()` | Generate auto-calculated bill and release the room |
| 4. Display Rooms | `displayRooms()` | Show all rooms with type, price, and current status |
| 5. Display Customers | `displayCustomers()` | List all current guests in the hotel |
| 6. Search Room | `searchRoom()` | Find any room instantly by room number |
| 7. Total Customers | `getTotalCustomers()` | Live count of guests currently in the hotel |
| 8. Manager Profile | `showManagerProfile()` | Display full manager identity — both admin and staff roles |
| 9. Manager Overtime | `managerOvertime()` | Log extra hours and update salary automatically |
| 10. Exit | — | Confirm and close the system |

---

## 7. Real-Life Problems & Solutions

### Problem 1 — A Manager Is Both Staff And Admin

**Problem:** Most systems force a person to be either staff or admin — never both. A hotel manager clocks in like staff AND has admin privileges. Designing this incorrectly duplicates his identity.

**Solution:** The `Manager` class inherits from both `AdminBase` and `StaffMember` using virtual inheritance. One person, two roles, zero duplication.

---

### Problem 2 — The Diamond Problem

**Problem:** `Manager` inheriting from both `AdminBase` and `StaffMember` — both of which inherit from `Person` — creates two copies of `Person` inside `Manager`. Calling `getName()` on Manager becomes ambiguous and causes a compiler error.

**Solution:** Both `AdminBase` and `StaffMember` use `virtual public Person`. The `Manager` constructor explicitly initialises `Person` once. One shared identity. No ambiguity.

---

### Problem 3 — Hotel Was Inheriting From Customer

**Problem:** The original design had `Hotel` inheriting from `Customer`. A Hotel is not a Customer — this is a logically incorrect IS-A relationship that pollutes the design.

**Solution:** `Customer` was removed from `Hotel`'s inheritance. `Hotel` now simply *contains* a list of customers via a `vector<Customer>` — correct HAS-A composition.

---

### Problem 4 — Double Booking

**Problem:** Two customers could potentially be assigned the same room, causing a real-world conflict.

**Solution:** Every room carries a boolean status. Before any booking, the system checks the status. If already booked, the request is refused immediately.

---

### Problem 5 — Duplicate Customer IDs

**Problem:** Two guests with the same ID makes it impossible to identify who owns which booking or bill.

**Solution:** Before confirming any booking, the system scans all existing customer records. If the entered ID already exists, it is rejected and a new one is requested.

---

### Problem 6 — Invalid Input Crashing The System

**Problem:** Hotel staff are not programmers. Typing "five" instead of "5", or entering a negative price, can crash a poorly designed system.

**Solution:** Dedicated `safeInt()` and `safeFloat()` functions wrap every input in error-checking logic. Invalid or out-of-range inputs are rejected and re-prompted automatically. The system never crashes from bad input.

---

### Problem 7 — Manual Billing Errors

**Problem:** Calculating bills manually — room price × days + food — is error-prone and causes disputes.

**Solution:** The `Bill` class calculates everything automatically at checkout using the customer's days and room price. The complete breakdown — room bill, food bill, total — is displayed instantly and accurately.

---

### Problem 8 — Unauthorized Access

**Problem:** Anyone walking up to the terminal could manage the hotel without accountability.

**Solution:** The `AdminBase::authenticate()` function requires a username and password before the system opens. Three failed attempts lock access completely.

---

### Problem 9 — No Live Guest Count

**Problem:** In an emergency — fire, evacuation — the hotel must know exactly how many people are inside at any moment.

**Solution:** `Customer::totalCustomers` is a static counter that increments on every check-in and decrements on every checkout via the destructor. The count is always live and accurate.

---

### Problem 10 — Staff Overtime Never Tracked

**Problem:** Managers frequently work beyond their shift hours but receive no acknowledgement or extra pay.

**Solution:** The `StaffMember::addOvertime()` function accepts extra hours, calculates the additional pay, and updates the salary immediately. Every extra hour is recorded and compensated.

---

### Problem 11 — Checkout On An Unbooked Room

**Problem:** A typo or miscommunication could trigger a checkout on an empty room — deleting a non-existent record or producing a ghost bill.

**Solution:** Before processing any checkout, the system confirms both that the room exists AND that its status is currently "booked." Only when both conditions are true does the checkout proceed.

---

## 8. Build & Run Instructions

### Prerequisites

- `g++` compiler with C++17 support
- Linux / macOS terminal or Windows with MinGW / WSL

### Compile & Run

```bash
# Clone the repository
git clone https://github.com/yourusername/hotel-management-system.git
cd hotel-management-system

# Compile
g++ -std=c++17 main.cpp -o hotel

# Run
./hotel
```

### Default Login Credentials

```
Username : admin
Password : 1234
```

> **Note:** In a production system, credentials would be stored as hashed values and verified securely.

---

## 9. Limitations & Future Work

| Limitation | Proposed Solution |
|---|---|
| Data lost when program closes | Implement file-based or database persistence |
| Only one manager supported | Add a list of manager profiles with individual logins |
| Food bill is a flat rate | Track actual food orders per customer |
| Console UI only | Build a Qt or web-based graphical frontend |
| Room types limited to single/double | Add suite, deluxe, and other categories |
| No date or time tracking | Integrate C++ `<ctime>` for real check-in/check-out timestamps |
| Linear search for rooms/customers | Replace `vector` search with `unordered_map` for O(1) lookup |

---

## 10. Authors

**Auyon Dip Sardar**
Roll: 2410043

Department of Electrical & Computer Engineering

Rajshahi University of Engineering & Technology (RUET)
Rajshahi – 6204, Bangladesh

**Nishit Ray**
Roll: 2410044

Department of Electrical & Computer Engineering

Rajshahi University of Engineering & Technology (RUET)
Rajshahi – 6204, Bangladesh

**Ahsan Ullah**
Roll: 2410045

Department of Electrical & Computer Engineering

Rajshahi University of Engineering & Technology (RUET)
Rajshahi – 6204, Bangladesh

---

<div align="center">

*This project was developed as part of the Object-Oriented Programming (ECE-1204) course requirement.*
*All OOP concepts are implemented in conformance with the C++17 standard.*

</div>
