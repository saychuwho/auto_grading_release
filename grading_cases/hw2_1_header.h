#ifndef HW2_1
#define HW2_1

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

#endif