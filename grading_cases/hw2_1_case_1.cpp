#include <iostream>
#include <string>

#include "hw2_1_header.h"

int main() { 
    cout << "=== Test Case 1: Multiple Product Types ===" << endl; 
    VendingMachine machine1;

    Product* p_soda = new Product("Soda", 1.25, 150); 
    Product* p_candy = new Product("Candy", 0.75, 100); 
    Product* p_sandwich = new Product("Sandwich", 2.50, 300); 
    Product* p_juice = new Product("Juice", 1.50, 200); 
     
    machine1.addProduct(p_soda); 
    machine1.addProduct(p_candy); 
    machine1.addProduct(p_sandwich); 
    machine1.addProduct(p_juice); 
     
    machine1.displayInventory(); 
     
    machine1.insertCoin(1.00); 
    machine1.insertCoin(0.25); 
    machine1.dispense("Soda"); 
     
    machine1.insertCoin(2.00); 
    machine1.insertCoin(0.50); 
    machine1.dispense("Sandwich"); 
     
    machine1.displayInventory(); 
     
    return 0; 
}