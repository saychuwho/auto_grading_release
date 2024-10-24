#include <iostream>
#include <string>

#include "hw2_1_header.h"


int main() { 
    cout << "=== Test Case 3: Different Coin Denominations ===" << endl; 
    VendingMachine machine3; 
     
    Product* p_chips = new Product("Chips", 1.50, 200); 
    Product* p_cookies = new Product("Cookies", 2.00, 250); 
     
    machine3.addProduct(p_chips); 
    machine3.addProduct(p_cookies); 
     
    machine3.insertCoin(0.25);  // Quarter 
    machine3.insertCoin(0.10);  // Dime 
    machine3.insertCoin(0.05);  // Nickel 
    machine3.insertCoin(1.00);  // Dollar 
    machine3.insertCoin(0.10);  // Dime 
     
    machine3.dispense("Chips"); 
     
    machine3.insertCoin(2.00); 
    machine3.dispense("Cookies"); 
     
    machine3.displayInventory(); 
     
    return 0; 
} 