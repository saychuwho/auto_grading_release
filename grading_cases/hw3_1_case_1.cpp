int main() { 
    cout << "=== Vending Machine Test Cases ===" << endl; 
    VendingMachine machine; 
    cout << "\n--- Test Case 1: Adding Products ---" << endl; 
    Product* cola = new Beverage("Cola", 1.50, 140); 
    Product* chips = new Snack("Chips", 1.00, 180); 
    Product* water = new Beverage("Water", 1.00, 0); 
    machine.addProduct(cola); 
    machine.addProduct(chips); 
    machine.addProduct(water); 
    machine.displayInventory(); 
    cout << "\n--- Test Case 2: Inserting Coins and Dispensing ---" << endl; 
    machine.insertCoin(1.00); 
    machine.insertCoin(0.50); 
    machine.dispense("Cola"); 
    machine.displayInventory(); 
    cout << "\n--- Test Case 3: Exact Change ---" << endl; 
    machine.insertCoin(1.00); 
    machine.dispense("Water"); 
    machine.displayInventory();
    cout << "\n--- Test Case 4: Insufficient Funds ---" << endl; 
    machine.insertCoin(0.50); 
    machine.dispense("Chips"); 
    machine.insertCoin(0.50); 
    machine.dispense("Chips"); 
    machine.displayInventory(); 
 
    cout << "\n--- Test Case 5: Non-existent Product ---" << endl; 
    machine.insertCoin(1.00); 
    machine.dispense("Candy"); 
 
    return 0; 
} 