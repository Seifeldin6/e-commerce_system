
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#define el '\n' 

class Product;
class ExpirableProduct;
class ShippableProduct;
class Customer;
class Shipping;
class Cart;


class Product {
protected:
    string name;
    double price;
    int quantity;

public:
    
    Product() : name(""), price(0.0), quantity(0) {}
    Product(const string& name, double price, int quantity)        : name(name), price(price), quantity(quantity) {}

    
    virtual ~Product() = default;

   
    string getName() const {
        return name;
    }

   
    double getPrice() const {
        return price;
    }


    int getQuantity() const {
        return quantity;
    }

    
    bool subQuantity(int val) {// O(1)
        if (quantity >= val) {
            quantity -= val;
            return true;
        }
        return false;
    }

    
    virtual bool isExpirable() const {
        return false;
    }

    
    virtual bool isShippable() const {
        return false;
    }

     virtual double getProductWeight() const {
        return 0.0;
    }

    virtual bool checkExp(const string& curDate) const {
        return false;
    }
};


class ExpirableProduct : public virtual Product {
private:
    string exp_date; // Format: YYYY-MM-DD

public:
    
    ExpirableProduct(const string& name, double price, int quantity, const string& exp_date)
        : Product(name, price, quantity), exp_date(exp_date) {}

    
    bool isExp(const string& curDate) const {
        return curDate > exp_date;
    }

    
    bool isExpirable() const override {
        return true;
    }

    bool checkExp(const string& curDate) const override {
        return isExp(curDate);
    }
};


class ShippableProduct : public virtual Product {
private:
    double weight;

public:
    
    ShippableProduct(const string& name, double price, int quantity, double weight)
        : Product(name, price, quantity), weight(weight) {}

   
    double getWeight() const {
        return weight;
    }

    
    bool isShippable() const override {
        return true;
    }


    double getProductWeight() const override {
        return weight;
    }
};


class ExpShipProduct : public ExpirableProduct, public ShippableProduct {
public:
    ExpShipProduct(const string& name, double price, int quantity, const string& exp_date, double weight)
        : ExpirableProduct(name, price, quantity, exp_date),
          ShippableProduct(name, price, quantity, weight) {}

  
    bool isExpirable() const override { return true; }
    bool isShippable() const override { return true; }

   
    double getProductWeight() const override { return ShippableProduct::getProductWeight(); }

  
    bool checkExp(const string& curDate) const override { return ExpirableProduct::checkExp(curDate); }
};


class Customer {
private:
    string name;
    double balance;

public:
    
    Customer(const string& name, double balance)
        : name(name), balance(balance) {}

   
    string getName() const {
        return name;
    }

    
    double getBalance() const {
        return balance;
    }

   
    bool subBalance(double val) {// O(1)
        if (balance >= val) {
            balance -= val;
            return true;
        }
        return false;
    }
};


class Shipping {
public:
    
    void ship(const vector<pair<string, double>>& items) const {// O(n log n) 
        if (items.empty()) {
            return; 
        }

        cout<<"** Shipment notice **"<<el;
        double totalWeight = 0;
       
        map<string, pair<int, double>> aggregatedItems;
        for (auto &item : items) { //O(nlog n)
            aggregatedItems[item.first].first++; // Inc quantity
            aggregatedItems[item.first].second += item.second; // Add weight
            totalWeight += item.second;
        }

        for (auto &pair : aggregatedItems) {
            cout<<pair.second.first <<"x "<<pair.first <<"\n";
            cout<<pair.second.second <<"g"<<endl;  
        }

        cout<<"Total package weight "<<totalWeight/1000<<"kg"<<endl;
    }
};


class Cart {
private:
  
    map<Product*, int> items;
    vector<Product*> allProducts; 

    Product* findproduct(const string& name)const{// O(n)
        for (auto &p : allProducts) {
            if (p->getName() == name) {
                return p;
            }
        }
        return nullptr;
    }

public:
    
    ~Cart() {
       allProducts.clear();
    }

    
    bool addProduct(Product* product, int quantity) {// O(n)
        if (quantity <= 0) {
            cerr<<"Error: Quantity must be positive."<<el;
            return false;
        }
        if (product == nullptr) {
            cerr<<"Error: Cannot add null product."<<el;
            return false;
        }

      
        Product* isExist = findproduct(product->getName());//o(n)
        if (isExist == nullptr) {

            allProducts.push_back(product);
            isExist = product;
        } 

        
        if (isExist->getQuantity() < quantity) {
            cerr << "Error: Insufficient stock for " << isExist->getName()<<". Available: "<<isExist->getQuantity()<<el;
            return false;
        }

        
        items[isExist] += quantity;
        cout<<quantity<<"x "<<isExist->getName()<<" added to cart."<<el;
        return true;
    }

    
    bool checkout(Customer& customer, const string& curDate) {
        if (items.empty()) {
            cerr <<"Error: Cart is empty."<<el;
            return false;
        }

        double subtotal = 0.0;
        vector<pair<string, double>> shippableItems;

        
        for (auto &pair : items) {// O(n)
            Product* product = pair.first;
            int &inCart = pair.second;

      
            if (product->getQuantity() < inCart) {
                cerr<<"Error: "<<product->getName()<<" is out of stock. Available: "<<product->getQuantity()<<el;
                return false;
            }

            
          if (product->isExpirable() && product->checkExp(curDate)) {
                cerr << "Error: "<<product->getName()<< " is expired."<<el;
                return false;
            }


            subtotal += product->getPrice() * inCart;

           
           if (product->isShippable()) {
                for (int i = 0; i < inCart; ++i) {
                    shippableItems.push_back({product->getName(), product->getProductWeight()});
                }
            }
        }

        double shippingFees = calculateShippingFee(shippableItems);
        double paidAmount = subtotal + shippingFees;

     
        if (customer.getBalance() < paidAmount) {
            cerr << "Error: Customer\"s balance is insufficient. Required: " << paidAmount << ", Available: "<<customer.getBalance()<<el;
            return false;
        }

      
        customer.subBalance(paidAmount);

       
        for (auto pair : items) {// O(n) update stock
            Product* product = pair.first;
            int inCart = pair.second;
            product->subQuantity(inCart);
        }

      
        Shipping shipNow;
        shipNow.ship(shippableItems);

        // Print checkout details
        cout << "** Checkout receipt **" << endl;
        for (auto &pair : items) {// O(n)
            Product* product = pair.first;
            int inCart = pair.second;
            cout << inCart << "x " << product->getName()<<el;
            cout << product->getPrice() * inCart<<el;
        }
        cout<< "---------------------" <<el;
        cout<< "Subtotal\t\t" << subtotal <<el;
        cout<< "Shipping\t\t" << shippingFees <<el;
        cout<< "Amount\t\t\t" << paidAmount << endl;
        cout<< "Customer current balance after payment: "<<customer.getBalance()<<endl;

        clearCart();
        return true;
    }

   
    double calculateShippingFee(const vector<pair<string, double>>& shippableItems) const {
        double totalWeight = 0.0;
        for (auto &item : shippableItems) {
            totalWeight += item.second;
        }

        double fee = totalWeight * 0.05;//Assuming 0.05 is the shipping rate per gramw ith a min fee of 10.00
        return max(fee, 10.00);
    }

  
    void clearCart() {
        items.clear();
    }
};


void printTestCaseHeader(const string& header) {
    cout << "\n========================================" << endl;
    cout << "TEST CASE: " << header << endl;
    cout << "========================================" << endl;
}

int main() {
    // Create some products 
    Product* tv = new ShippableProduct("TV", 1000.0, 5, 15000.0); // 15kg
    Product* mobile = new Product("Mobile", 500.0, 10); // Not shippable, not expirable
    Product* cheese = new ExpirableProduct("Cheese", 50.0, 20, "2025-07-01"); // Expired
    Product* biscuits = new ExpirableProduct("Biscuits", 30.0, 15, "2025-08-01"); // Not expired
    Product* scratchCard = new Product("Scratch Card", 10.0, 100); // Not shippable, not expirable
    Product* freshMilk = new ExpShipProduct("Fresh Milk", 5.0, 10, "2025-07-06", 500.0); // Not expired, shippable (500g)
    Product* heavyBook = new ShippableProduct("Heavy Book", 25.0, 8, 2000.0); // 2kg
     // Create some customers
    Customer alice("Alice", 2000.0);
    Customer bob("Bob", 100.0);
    Customer charlie("Charlie", 5000.0);

    Cart cart;
    string curDate = "2025-07-04"; // Current date for testing

    // --- Test Case 1: Successful Checkout with Mixed Products (Shippable, Expirable, Normal) ---
    printTestCaseHeader("Successful Checkout with Mixed Products");
    cart.addProduct(tv, 1);
    cart.addProduct(biscuits, 2);
    cart.addProduct(scratchCard, 5);
    cart.addProduct(freshMilk, 1);
    cart.checkout(alice, curDate);
    cout << "Alice\"s balance after checkout: " << alice.getBalance()<<endl;
    cout << "TV quantity after checkout: " << tv->getQuantity()<<endl;
    cout << "Biscuits quantity after checkout: " << biscuits->getQuantity()<<endl;
    cout << "Scratch Card quantity after checkout: " << scratchCard->getQuantity()<<endl;
    cout << "Fresh Milk quantity after checkout: " << freshMilk->getQuantity()<<endl;

    // --- Test Case 2: Adding more than available quantity ---
    printTestCaseHeader("Adding more than available quantity");
    cart.clearCart(); // Clear cart from previous test
    cart.addProduct(mobile, 12); // Mobile has 10 available, trying to add 12
    cart.checkout(bob, curDate); 

    // --- Test Case 3: Checkout with insufficient customer balance ---
    printTestCaseHeader("Checkout with insufficient customer balance");
    cart.clearCart();
    cart.addProduct(new ShippableProduct("TV", 1000.0, 5, 15000.0), 2); // Price 2000
    cart.checkout(bob, curDate); // Bob has 100, needs 2000 + shipping
    cout << "Bob\"s balance after failed checkout: " << bob.getBalance()<<el;

    // --- Test Case 4: Checkout with an expired product ---
    printTestCaseHeader("Checkout with an expired product");
    cart.clearCart();
    cart.addProduct(new ExpirableProduct("Cheese", 50.0, 20, "2025-07-01"), 1);
    cart.checkout(charlie, curDate); // Cheese is expired
    cout << "Charlie\"s balance after failed checkout: " << charlie.getBalance() <<el;

    // --- Test Case 5: Checkout with an empty cart ---
    printTestCaseHeader("Checkout with an empty cart");
    cart.clearCart();
    cart.checkout(alice, curDate);

    // --- Test Case 6: Multiple items of the same shippable product ---
    printTestCaseHeader("Multiple items of the same shippable product");
    cart.clearCart();
    cart.addProduct(new ShippableProduct("Heavy Book", 25.0, 8, 2000.0), 3);
    cart.checkout(charlie, curDate);
    cout << "Charlie\"s balance after checkout: " << charlie.getBalance()<<el;
    cout << "Heavy Book quantity after checkout: " << heavyBook->getQuantity()<<el;

    // --- Test Case 7: Product becomes out of stock after adding to cart but before checkout ---
    printTestCaseHeader("Product becomes out of stock before checkout");
    cart.clearCart();
    cart.addProduct(new Product("Mobile", 500.0, 10), 5);
    mobile->subQuantity(6); 
    cart.checkout(alice, curDate); 
    cout << "Alice\"s balance after failed checkout: " << alice.getBalance() <<el;

    return 0;
}


