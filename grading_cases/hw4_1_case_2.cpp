int main() {
    cout << "==========Part 1==========" << endl;
    VendingMachine machine;
    // Add some products using operator overloading
    machine + new Beverage("Cola", 1.50, 330);
    machine + new Snack("Chips", 1.00, 150);
    machine + new Beverage("Water", 1.00, 500);
    machine.displayInventory();
    cout << "==========Part 2==========" << endl;
    // Demonstrate item-specific dispensing with operator overloading for coins
    machine + 1.00 + 0.50; // Insert $1.50
    machine.dispense("Cola"); // Should dispense Cola
    machine + 1.00; // Insert $1.00
    machine.dispense("Water"); // Should dispense Water
    cout << "==========Part 3==========" << endl;
    machine + 0.50; // Insert $0.50
    machine.dispense("Chips"); // Should say insufficient funds
    machine + 0.50; // Insert additional $0.50
    machine.dispense("Chips"); // Should dispense Chips
    cout << "==========Part 4==========" << endl;
    machine + 1.00; // Insert $1.00
    machine.dispense("Candy"); // Should say product not available
    machine.displayInventory();
    cout << "==========Part 5==========" << endl;
    // Add more products
    machine + new Snack("Chocolate", 1.25, 200);
    machine + new Beverage("Orange Juice", 1.75, 250);
    machine.displayInventory();
    cout << "==========Part 6==========" << endl;
    // Dispense remaining products
    machine + 2.00;
    machine.dispense("Chocolate");
    machine + 2.00;
    machine.dispense("Orange Juice");
    machine.displayInventory();
    return 0;
}