# E-commerce Product Management System

This C++ project simulates a basic e-commerce product management system, demonstrating object-oriented programming principles, inheritance, and polymorphism. It handles various product types, customer balances, shipping, and cart functionalities.

## Features

- **Product Management**: Base `Product` class with common attributes like name, price, and quantity.
- **Expirable Products**: `ExpirableProduct` class inherits from `Product` and adds an expiration date.
- **Shippable Products**: `ShippableProduct` class inherits from `Product` and adds weight for shipping calculations.
- **Expirable and Shippable Products**: `ExpirableShippableProduct` class demonstrates multiple inheritance, allowing products to be both expirable and shippable.
- **Customer Management**: `Customer` class to manage customer names and balances.
- **Shopping Cart**: `Cart` class to add products, manage quantities, and perform checkout.
- **Shipping Calculation**: `Shipping` class to calculate shipping fees based on product weight.
- **Stock Management**: Products' quantities are updated upon successful checkout.
- **Expiration Check**: Products are checked for expiration during checkout.
- **Insufficient Stock/Balance Handling**: Error handling for insufficient stock or customer balance.

## How to Compile and Run

To compile and run this project, you will need a C++ compiler (like g++).

1.  **Save the code**: Save the provided C++ code into a file named `main.cpp` (or any other `.cpp` extension).

2.  **Compile**: Open your terminal or command prompt, navigate to the directory where you saved the file, and compile using g++:

    ```bash
    g++ -std=c++11 main.cpp -o main
    ```

3.  **Run**: Execute the compiled program:

    ```bash
    ./main
    ```

## Class Structure Overview

-   `Product`: Base class for all products.
-   `ExpirableProduct`: Inherits from `Product`, adds expiration date.
-   `ShippableProduct`: Inherits from `Product`, adds weight.
-   `ExpirableShippableProduct`: Inherits from both `ExpirableProduct` and `ShippableProduct`.
-   `Customer`: Manages customer details.
-   `Shipping`: Handles shipping logic.
-   `Cart`: Manages shopping cart operations and checkout.

## Test Cases

The `main` function includes several test cases to demonstrate the system's functionality:

-   Successful Checkout with Mixed Products
-   Adding more than available quantity
-   Checkout with insufficient customer balance
-   Checkout with an expired product
-   Checkout with an empty cart
-   Multiple items of the same shippable product
-   Product becomes out of stock before checkout


