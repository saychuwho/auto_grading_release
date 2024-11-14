//2024-fall Assignment 3 (Vending Machine)

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
protected:  //MODIFIED
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
    virtual ~Product();
    /**
     * @brief Get the name of the product
     * @return string The product's name
     */
    virtual string getName() const { return p_name; } // Changed to return by value
    /**
     * @brief Get the price of the product
     * @return double The product's price
     */
    virtual double getPrice() const { return p_price; }
    /**
     * @brief Get the ID of the product
     * @return int The product's ID
     */
    virtual int getId() const { return id; }
    /**
     * @brief Describe the product (print its details)
     */
    virtual void describe() const {
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

// Derived Product classes 
/**
 * @class Beverage
 * @brief Represents a beverage product, derived from Product
 */
class Beverage : public Product {
//private:
//    double volume; ///< Volume of the beverage in ml

public:
    /**
    * @brief Construct a new Beverage object
    * @param name The name of the beverage
    * @param price The price of the beverage
    * @param volume The volume of the beverage in ml
    */
    Beverage(string name, double price, double calorie);
    /**
     * @brief Describe the beverage (print its details)
     */
    void describe() const override {
        cout << "Beverage: " << getName() << " (ID: " << getId()
            << ", Price: $" << getPrice() << ", Calories: " << this->p_calorie << " calories)\n";
    }
};
Beverage::Beverage(string name, double price, double calorie) : Product(name, price, calorie) {
    cout << "Beverage created: " << getName() << " (" << this->p_calorie << " calories)\n"; //modified
}

/**
 * @class Snack
 * @brief Represents a snack product, derived from Product
 */
class Snack : public Product {
//private:
//    int calories; ///< Calorie content of the snack

public:
    /**
     * @brief Construct a new Snack object
     * @param name The name of the snack
     * @param price The price of the snack
     * @param calories The calorie content of the snack
     */
    Snack(string name, double price, double calorie);
    /**
     * @brief Describe the snack (print its details)
     */
    void describe() const override {
        cout << "Snack: " << getName() << " (ID: " << getId()
            << ", Price: $" << getPrice() << ", Calories: " << this->p_calorie << " calories)\n";
    }
};

Snack::Snack(string name, double price, double calorie): Product(name, price, calorie) {
    cout << "Snack created: " << getName() << " (" << this->p_calorie << " calories)\n";
}
class State {
protected:
    string s_name;  ///Store state name
    VendingMachine* machine;  ///< Pointer to the associated VendingMachine 

public:
    /**
     * @brief Construct a new State object
     * @param machine Pointer to the associated VendingMachine
     */
    State(VendingMachine* machine); 

    /**
     * @brief Get the name of the state
     * @return string The state's name
     */
    virtual string getName() const = 0;
    /**
     * @brief Destroy the State object
     */
    virtual ~State(); 

    /**
     * @brief Handle coin insertion 
     */
    virtual void insertCoin(double coin) = 0;
    /**
     * @brief Handle coin ejection 
     */
    virtual void ejectCoin() = 0;
    /**
     * @brief Handle product dispensing 
     * @param productName Name of the product to dispense
     */
    virtual void dispense(string productName) = 0; 
};


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

    //MODIFIED
    void addCoinValue(double coin) { coinValue += coin; }
};

State::State(VendingMachine* t_machine) : machine(t_machine) {
    //cout << "Constructing State\n";
    //cout << "[Constructor] Constructing State: " << s_name << "\n";
}

//DELETED
//State::State(string name) {
//    s_name = name;
//    cout << "[Constructor] Constructing State: " << s_name << "\n";
//}

State::~State() {
    cout << "[Destructor] Destructing State: " << s_name << "\n";
}

// State classes
/**
 * @class NoCoinState (MODIFIED)
 * @brief Represents the state when no coin is inserted
 */
class NoCoinState : public State {
public:
    /**
     * @brief Construct a new NoCoinState object
     * @param machine Pointer to the associated VendingMachine
     */
    NoCoinState(VendingMachine* t_machine);
    /**
     * @brief Destroy the NoCoinState object
     */
    ~NoCoinState() override;
    void insertCoin(double coin) override;
    void ejectCoin() override;
    void dispense(string productName) override;
    string getName() const override { return this->s_name; }
};

NoCoinState::NoCoinState(VendingMachine* t_machine) : State(t_machine) {
    this->s_name = "No Coin";
    cout << "[Constructor] Constructing State: " << s_name << "\n";
}
NoCoinState::~NoCoinState()  {
    //cout << "[Destructor] Destructing State: " << s_name << "\n";
}
// State method implementations 
void NoCoinState::insertCoin(double coin) {
    //cout << "Coin inserted: $" << machine->getCoinValue() << "\n";    
    machine->setCoinInserted(true);
    machine->addCoinValue(coin);
    machine->setState(machine->getHasCoinState());
}


void NoCoinState::ejectCoin() { 
    cout << "No coin to eject\n";
}

void NoCoinState::dispense(string productName) { // Changed to pass by value
    cout << "Insert a coin first\n";
}

/**
 * @class HasCoinState
 * @brief Represents the state when a coin is inserted
 */
class HasCoinState : public State {
public:
    /**
     * @brief Construct a new HasCoinState object
     * @param machine Pointer to the associated VendingMachine
     */
    HasCoinState(VendingMachine* t_machine);
    /**
     * @brief Destroy the HasCoinState object
     */
    ~HasCoinState() override;
    void insertCoin(double coin) override;
    void ejectCoin() override;
    void dispense(string productName) override; // Changed to pass by value
    string getName() const override { return this->s_name; }
};

HasCoinState::HasCoinState(VendingMachine* t_machine) : State(t_machine) {
    this->s_name = "Has Coin";
    cout << "[Constructor] Constructing State: " << s_name << "\n";
}
HasCoinState::~HasCoinState()  {
    //cout << "[Destructor] Destructing State: " << s_name << "\n";
}

void HasCoinState::insertCoin(double coin) {
    
    machine->addCoinValue(coin);
    //cout << "Additional coin inserted. Total: $" << machine->getCoinValue() << "\n";
}

void HasCoinState::ejectCoin() {
    cout << "Coins ejected: $" << machine->getCoinValue() << "\n";
    machine->resetCoinValue();
    machine->setCoinInserted(false);
    machine->setState(machine->getNoCoinState());
}

void HasCoinState::dispense(string productName) { // Changed to pass by value
    if (machine->isProductAvailable(productName)) {
        double productPrice = machine->getProductPrice(productName);
        if (machine->getCoinValue() >= productPrice) {
            Product* dispensedProduct = machine->removeProduct(productName);
            if (dispensedProduct) {
                double change = machine->getCoinValue() - productPrice;
                if (change > 0) {
                    cout << "Change returned: $" << change << "\n";
                }
                machine->resetCoinValue();
                machine->setCoinInserted(false);
                if (machine->getInventoryCount() > 0) {
                    machine->setState(machine->getNoCoinState());
                }
                else {
                    machine->setState(machine->getSoldOutState());
                }
                delete dispensedProduct;
            }
        }
        else {
            cout << "Insufficient funds. Please insert more coins.\n";
            cout << "Current balance: $" << machine->getCoinValue() << ", Required: $" << productPrice << "\n";
        }
    }
    else {
        cout << "Product " << productName << " is not available\n";
    }
}
/**
 * @class SoldOutState 
 * @brief Represents the state when the machine is out of products
 */
class SoldOutState : public State {
public:
    /**
    * @brief Construct a new SoldOutState object
    * @param machine Pointer to the associated VendingMachine
    */
    SoldOutState(VendingMachine* t_machine);
    /**
     * @brief Destroy the SoldOutState object
     */
    ~SoldOutState() override;
    void insertCoin(double coins) override;
    void ejectCoin() override;
    void dispense(string productName) override; // Changed to pass by value
    string getName() const override { return this->s_name; }
};
SoldOutState::SoldOutState(VendingMachine* t_machine) : State(t_machine) {
    this->s_name = "Sold Out";
    cout << "[Constructor] Constructing State: " << s_name << "\n";
}
SoldOutState::~SoldOutState()  {
    //cout << "[Destructor] Destructing State: " << s_name << "\n";
}

void SoldOutState::insertCoin(double coins) {
    //cout << "Machine is sold out. Coin ejected: $" << machine->getCoinValue() << "\n";
    cout << "SOLD OUT: No additional coin accepted" << endl;
    machine->resetCoinValue();
}

void SoldOutState::ejectCoin() {
    cout << "No coin to eject\n";
}

void SoldOutState::dispense(string productName) { // Changed to pass by value
    cout << "No product to dispense\n";
}


// State classes

// VendingMachine method implementations
VendingMachine::VendingMachine() : hasCoin(false), coinValue(0.0), num_of_products(0) {
    cout << "[Constructor] Constructing VendingMachine\n";

    noCoinState = new NoCoinState(this);  
    hasCoinState = new HasCoinState(this); 
    soldOutState = new SoldOutState(this); 
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
    currentState->insertCoin(coin);
    printState("Insert Coin");
}

void VendingMachine::ejectCoin() {
    currentState->ejectCoin();
    printState("Eject Coin");
}

void VendingMachine::dispense(string productName) {
    currentState->dispense(productName);
    printState("Dispense");
}

Product* VendingMachine::removeProduct(string productName) {
    for (int i = 0; i < this->num_of_products; i++) {
        if (this->inventory[i]->getName() == productName) {
            Product* temp_product = this->inventory[i]; //Store the product pointer temporalily
            //traverse the rest of the products, and shift all to the left
            for (int j = i + 1; j < this->num_of_products; j++) {
                this->inventory[j - 1] = this->inventory[j];
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
    for (int i = 0; i < this->num_of_products; i++) {
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
    cout << "Total items: " << this->num_of_products << "\n";
}


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
