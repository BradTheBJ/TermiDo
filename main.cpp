#include <iostream>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> numbers = {"To do", "1", "2", "3"};

    while (true) {
        // Print the current contents
        std::cout << "\nCurrent list:\n";
        for (const std::string& number : numbers) {
            std::cout << number << "\n";
        }

        // Prompt user
        std::cout << "\nEnter a new item to add (or type 'exit' to quit): ";

        std::string input;
        std::cin >> input;

        // Check for exit condition
        if (input == "exit") {
            std::cout << "Exiting program.\n";
            break;
        }

        // Add input to the list
        numbers.push_back(input);

        std::cout << "Item added!\n";
    }

    return 0;
}
