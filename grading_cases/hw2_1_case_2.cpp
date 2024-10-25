int main() { 
    cout << "=== Test Case 2: Fill to Capacity ===" << endl; 
    VendingMachine machine2; 
     
    for (int i = 1; i <= 10; i++) { 
        string name = "Product" + to_string(i); 
        Product* p = new Product(name, 1.00, 100); 
        machine2.addProduct(p); 
    } 
     
    machine2.displayInventory(); 
     
    for (int i = 0; i < 5; i++) { 
        machine2.insertCoin(1.00); 
        machine2.dispense("Product" + to_string(i+1)); 
    } 
     
    machine2.displayInventory(); 
     
    return 0; 
} 