#include <iostream>
#include <string>
#include "hw2_1_header.h"

using namespace std;

int Product::nextId = 1;

Product::Product(string name, double price, double p_calorie) : p_name(name), p_price(price), p_calorie(p_calorie) {
    id = nextId++;
    cout << "[Constructor] Product created: " << p_name << " (ID: " << id << ", Price: $" << p_price << ")\n";
}
Product::~Product() {
    cout << "[Destructor] Product destroyed: " << p_name << " (ID: " << id << ")\n";
}

State::State(string name) {
    s_name = name;
    cout << "[Constructor] Constructing State: " << s_name << "\n";
}
State::~State() {
    cout << "[Destructor] Destructing State: " << s_name << "\n";
}

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