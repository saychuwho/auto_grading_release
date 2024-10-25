int main() { 
    cout << "==========Part 1==========" << endl; 
    VendingMachine machine; //Create a VendingMachine instance 
    Product* p_cola = new Product("Cola", 1.50, 330);  //Create a new product 
    Product* p_chips = new Product("Chips", 1.00, 150); //Create a new product 
    Product* p_water = new Product("Water", 1.00, 0); //Create a new product 
    cout << "==========Part 2==========" << endl; 
    //Add three products 
    machine.addProduct(p_cola); 
    machine.addProduct(p_chips); 
    machine.addProduct(p_water); 
    //Display inventory 
    machine.displayInventory(); 
 
    cout << "==========Part 3==========" << endl; 
    //Insert coin and dispense 
    machine.insertCoin(1.00); 
    machine.insertCoin(0.50); 
    machine.dispense("Cola"); 
         
    cout << "==========Part 4==========" << endl; 
    machine.insertCoin(1.00); 
    machine.dispense("Water"); 
 
    cout << "==========Part 5==========" << endl; 
    machine.insertCoin(0.50); 
    machine.dispense("Chips"); // Should say insufficient funds 
 
    machine.insertCoin(0.50); 
    machine.dispense("Chips"); 
 
    cout << "==========Part 6==========" << endl; 
    machine.insertCoin(1.00); 
    machine.dispense("Candy"); // Should say product not available 
 
    machine.displayInventory(); 
 
    cout << "==========Part 7==========" << endl; 
    Product* p_chocolate = new Product("Chocolate", 1.25, 200); 
    Product* p_juice = new Product("Orange Juice", 1.75, 250); 
 
    machine.addProduct(p_chocolate); 
    machine.addProduct(p_juice); 
 
    machine.displayInventory(); 
 
    cout << "==========Part 8==========" << endl; 
    machine.insertCoin(2.00); 
    machine.dispense("Chocolate"); 
    machine.insertCoin(2.00); 
    machine.dispense("Orange Juice"); 
 
    machine.displayInventory(); 
 
    return 0; 
} 