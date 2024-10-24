#include <iostream>
#include <string>

#include "hw2_1_header.h"

int main() { 
    cout << "=== Test Case 5: Exact Change and Overpayment ===" << endl; 
    VendingMachine machine5; 
     
    Product* p_snack = new Product("Snack Bar", 1.75, 180); 
    Product* p_fruit = new Product("Fresh Fruit", 2.25, 80); 
    Product* p_yogurt = new Product("Yogurt", 1.50, 120); 
     
    machine5.addProduct(p_snack); 
    machine5.addProduct(p_fruit); 
    machine5.addProduct(p_yogurt); 
     
    machine5.displayInventory(); 
     
    // Exact change 
    machine5.insertCoin(1.00); 
    machine5.insertCoin(0.50); 
    machine5.insertCoin(0.25); 
    machine5.dispense("Snack Bar"); 
     
    // Overpayment 
    machine5.insertCoin(2.00); 
    machine5.insertCoin(1.00); 
    machine5.dispense("Fresh Fruit"); 
     
    // Insufficient funds, then exact change 
    machine5.insertCoin(1.00); 
    machine5.dispense("Yogurt"); 
    machine5.insertCoin(0.50); 
    machine5.dispense("Yogurt"); 
     
    machine5.displayInventory(); 
     
    return 0; 
} 