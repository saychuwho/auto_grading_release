//2024-fall Assignment 2 (Vending Machine)
#include <iostream>
#include <string>

using namespace std;

class VendingMachine; // Forward declaration

// Base Product class
/**
 * @class Product
 * @brief Base class for all products in the vending machine.
 */
class Product {
private:
    static int nextId; ///< Static counter for generating unique product IDs
    int id; ///< Unique identifier for the product
    string p_name; ///< Name of the product
    double p_price;   ///< Price of the product
    double p_calorie; ///calorie of the product

public:
    Product(string name, double price, double p_calorie);
    /**
     * @brief Destroy the Product object
     */
    ~Product();
    /**
     * @brief Get the name of the product
     * @return string The product's name
     */
    string getName() const { return p_name; } // Changed to return by value
    /**
     * @brief Get the price of the product
     * @return double The product's price
     */
    double getPrice() const { return p_price; }
    /**
     * @brief Describe the product (print its details)
     */
    void describe() const {
        cout << "Product: " << p_name << " (ID: " << id << ", Price: $" << p_price << ")\n";
    }
};

int Product::nextId = 1;

Product::Product(string name, double price, double p_calorie) : p_name(name), p_price(price), p_calorie(p_calorie) {
    id = nextId++;
    cout << "[Constructor] Product created: " << p_name << " (ID: " << id << ", Price: $" << p_price << ")\n";
}
Product::~Product() {
    cout << "[Destructor] Product destroyed: " << p_name << " (ID: " << id << ")\n";
}

class State {
private:
    string s_name;  ///Store state name
public:
    /**
     * @brief Construct a new State object
     * @param machine Pointer to the associated VendingMachine
     */
    State(string name);

    /**
     * @brief Get the name of the state
     * @return string The state's name
     */
    string getName() const { return s_name; };
    /**
     * @brief Destroy the State object
     */
    ~State();
};

State::State(string name) {
    s_name = name;
    cout << "[Constructor] Constructing State: " << s_name << "\n";
}
State::~State() {
    cout << "[Destructor] Destructing State: " << s_name << "\n";
}

// VendingMachine class
/**
 * @class VendingMachine
 * @brief Represents the vending machine
 */
class VendingMachine {
private:
    State* noCoinState; ///< Pointer to the NoCoinState
    State* hasCoinState;    ///< Pointer to the HasCoinState
    State* soldOutState;    ///< Pointer to the SoldOutState

    State* currentState;    ///< Pointer to the current state. This pointer will be updated upon transitions.    
    const int MAX_NUM_PRODUCT = 10; ///Maximum number of products
    Product* inventory[10];  ///< Vector of products in the machine
    int num_of_products; ///Store the number of products in the vending machine
    bool hasCoin;   ///< Flag indicating if a coin is inserted
    double coinValue;   ///< Value of inserted coins

    /**
     * @brief Print the current state and action
     * @param action The action being performed
     */
    void printState(string action) const { 
        cout << "Action: " << action << " | Current State: " << currentState->getName() << " | Coin Value: " << this->coinValue << "\n";
    }

public:
    /**
     * @brief Construct a new VendingMachine object
     */
    VendingMachine();
    /**
     * @brief Destroy the VendingMachine object
     */
    ~VendingMachine();
    /**
     * @brief Set the current state of the machine
     * @param state Pointer to the new state
     */
    void setState(State* state);
    /**
     * @brief Handle coin insertion
     */
    void insertCoin(double coin);

    /**
     * @brief Eject inserted coins
     */
    void ejectCoin();
    /**
     * @brief Dispense a product
     * @param productName Name of the product to dispense
     */
    void dispense(string productName);

    /**
     * @brief Remove a product from inventory
     * @param productName Name of the product to remove
     * @return Product* Pointer to the removed product, or nullptr if not found
     */
    Product* removeProduct(string productName);

    void addProduct(Product* product);

    /**
     * @brief Check if a product is available
     * @param productName Name of the product to check
     * @return true if the product is available, false otherwise
     */
    bool isProductAvailable(string productName) const;
    /**
     * @brief Get the price of a product
     * @param productName Name of the product
     * @return double Price of the product, or 0.0 if not found
     */
    double getProductPrice(string productName) const;

   
    /**
     * @brief Display the current inventory
     */
    void displayInventory() const;

    /**
    * @brief Get the number of products in inventory
    * @return int Number of products
    */
    int getInventoryCount() const { return num_of_products; }

    /**
     * @brief Get the NoCoinState
     * @return State* Pointer to the NoCoinState
     */
    State* getNoCoinState() const { return noCoinState; }
    /**
     * @brief Get the HasCoinState
     * @return State* Pointer to the HasCoinState
     */
    State* getHasCoinState() const { return hasCoinState; }
    /**
     * @brief Get the SoldOutState
     * @return State* Pointer to the SoldOutState
     */
    State* getSoldOutState() const { return soldOutState; }

    /**
     * @brief Check if a coin has been inserted
     * @return true if a coin is inserted, false otherwise
     */
    bool hasInsertedCoin() const { return hasCoin; }
    /**
     * @brief Set the coin inserted flag
     * @param inserted New value for the coin inserted flag
     */
    void setCoinInserted(bool inserted) { hasCoin = inserted; }
    /**
     * @brief Get the value of inserted coins
     * @return double Value of inserted coins
     */
    double getCoinValue() const { return coinValue; }
    /**
     * @brief Reset the value of inserted coins to 0
     */
    void resetCoinValue() { coinValue = 0.0; }    
};

// State classes

// VendingMachine method implementations
VendingMachine::VendingMachine() : hasCoin(false), coinValue(0.0), num_of_products(0) {
    cout << "[Constructor] Constructing VendingMachine\n";
    
    noCoinState = new State("No Coin");
    hasCoinState = new State("Has Coin");
    soldOutState = new State("Sold Out");
    currentState = noCoinState;
    printState("Initialization");
}

VendingMachine::~VendingMachine() {
    cout << "[Destructor] Destructing VendingMachine\n";
    delete noCoinState;
    delete hasCoinState;
    delete soldOutState;
    for (int i = 0; i < this->num_of_products; i++) {
        delete this->inventory[i];
    }
}

void VendingMachine::setState(State* state) {
    currentState = state;
    printState("State Changed");
}

void VendingMachine::insertCoin(double coin) {
    
    if (currentState->getName() == "No Coin") {
        //Perform action
        this->coinValue += coin;
        //State change
        this->setState(this->hasCoinState);
    }
    else if (currentState->getName() == "Has Coin") {
        //Perform action
        this->coinValue += coin;
        //State change
        
    }
    else if (currentState->getName() == "Sold Out") {
        //Perform action
        cout << "SOLD OUT: No additional coin accepted" << endl;
        //State change
    }
    else {
        cout << "Error: No matched state" << endl;
    }
    printState("Insert Coin");
}

void VendingMachine::ejectCoin() {
    
    if (currentState->getName() == "No Coin") {
        //Perform action
        cout << "No coin to eject\n";
        //State change
    }
    else if (currentState->getName() == "Has Coin") {
        //Perform action
        cout << "Coins ejected: $" << this->getCoinValue() << "\n";
        this->resetCoinValue();
        this->setCoinInserted(false);
        //State change
        this->setState(this->noCoinState);
    }
    else if (currentState->getName() == "Sold Out") {
        //Perform action
        cout << "No coin to eject\n";
        //State change
    }
    else {
        cout << "Error: No matched state" << endl;
    }
    printState("Eject Coin");
}

void VendingMachine::dispense(string productName) { 
    
    if (currentState->getName() == "No Coin") {
        //Perform action
        cout << "Insert a coin first\n";
        //State change
    }
    else if (currentState->getName() == "Has Coin") {
        //Perform action
        if (this->isProductAvailable(productName)) {
            double productPrice = this->getProductPrice(productName);
            if (this->getCoinValue() >= productPrice) {
                Product* dispensedProduct = this->removeProduct(productName);
                if (dispensedProduct) {
                    double change = this->getCoinValue() - productPrice;
                    if (change > 0) {
                        cout << "Change returned: $" << change << "\n";
                    }
                    this->resetCoinValue();
                    this->setCoinInserted(false);
                    if (this->getInventoryCount() > 0) {
                        this->setState(this->noCoinState);
                    }
                    else {
                        this->setState(this->soldOutState);
                    }
                    delete dispensedProduct;
                }
            }
            else {
                cout << "Insufficient funds. Please insert more coins.\n";
                cout << "Current balance: $" << this->getCoinValue() << ", Required: $" << productPrice << "\n";
            }
        }
        else {
            cout << "Product " << productName << " is not available\n";
        }
        //State change
        
    }
    else if (currentState->getName() == "Sold Out") {
        //Perform action
        cout << "No product to dispense\n";
        //State change
    }
    else {
        cout << "Error: No matched state" << endl;
    }
    printState("Dispense");
    
}

Product* VendingMachine::removeProduct(string productName) { 
    for (int i = 0; i < this->num_of_products; i++) {
        if (this->inventory[i]->getName() == productName) {
            Product* temp_product = this->inventory[i]; //Store the product pointer temporalily
            //traverse the rest of the products, and shift all to the left
            for (int j = i+1; j < this->num_of_products; j++) {
                this->inventory[j-1] = this->inventory[j];
            }
            this->num_of_products--; //Reduce the number of product by 1
            //cout << "[removeProduct] A product is removed: " << temp_product->getName() << endl;
            return temp_product;
        }
    }
    //If nothing is found
    cout << "[removeProduct] No product to remove" << endl;
    return nullptr;

}

//Add product
void VendingMachine::addProduct(Product* product) {    

    if (this->num_of_products == this->MAX_NUM_PRODUCT) {
        cout << "[addProduct] Inventory is full" << endl;
        return;
    }

    //Perform actions for all states (No Coin, Has Coin, Sold Out)
    this->inventory[this->num_of_products] = product;   //Add the new product at the end of the inventory
    this->num_of_products++;        //Increase the number of products
    //cout << "[addProduct] Added a new product: " << this->inventory[num_of_products - 1]->getName() << endl;
    
    //State change (Only for Sold Out state)
    if (currentState->getName() == "Sold Out") {
        this->setState(this->noCoinState);
    }
    return;
}


bool VendingMachine::isProductAvailable(string productName) const { 
    for (int i = 0 ; i < this->num_of_products; i++){
        if (this->inventory[i]->getName() == productName) {
            return true; //Found the product
        }
    }
    return false; //Cannot find the product
}

double VendingMachine::getProductPrice(string productName) const {
    for (int i = 0; i < this->num_of_products; i++) {
        if (this->inventory[i]->getName() == productName) {
            return this->inventory[i]->getPrice(); //Found the product
        }
    }
    return 0.0; //Cannot find the product

}

void VendingMachine::displayInventory() const {
    cout << "Current Inventory:\n";
    for (int i = 0; i < this->num_of_products; i++) {
        this->inventory[i]->describe();
    }   
    cout << "Total items: " << this->num_of_products<< "\n";
}


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