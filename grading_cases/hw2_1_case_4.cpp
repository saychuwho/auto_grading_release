#include <iostream>
#include <string>

#include "hw2_1_header.h"

int main() { 
    cout << "=== Test Case 4: State Transitions and Coin Handling ===" << endl; 
    VendingMachine machine4; 
     
    Product* p_coffee = new Product("Coffee", 2.00, 5); 
    Product* p_tea = new Product("Tea", 1.50, 2); 
     
    machine4.addProduct(p_coffee); 
    machine4.addProduct(p_tea); 
     
    machine4.displayInventory(); 
     
    // Test coin insertion 
    machine4.insertCoin(1.00); 
    machine4.insertCoin(0.50); 
     
    // Test insufficient funds 
    machine4.dispense("Coffee"); 
     
    // Test correct amount 
    machine4.insertCoin(0.50); 
    machine4.dispense("Coffee"); 
     
    machine4.displayInventory(); 
     
    // Test transition to sold out state 
    machine4.insertCoin(1.00); 
    machine4.insertCoin(0.50); 
    machine4.dispense("Tea"); 
     
    // Try to insert coin in sold out state 
    machine4.insertCoin(1.00); 
     
    // Add a new product to transition out of sold out state 
    Product* p_soda = new Product("Soda", 1.25, 150); 
    machine4.addProduct(p_soda); 
     
    machine4.displayInventory(); 
     
    // Test dispense with overpayment 
    machine4.insertCoin(2.00); 
    machine4.dispense("Soda"); 
     
    machine4.displayInventory(); 
     
    return 0; 
} 