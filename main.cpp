#include <iostream>
#include <optional>
#include <string>
using namespace std;

/* This is a calculator that allows binary operations:

    - Addition
    - Subtraction
    - Multiplication
    - Division
    - Modulo remainder

  - The user should be asked:
  - The first number (must be an integer)
  - The operation (must be a char from a given list of operation chars)
  - The second number.

  - They will then be shown the result, with the option to start again.

    - We will need a reusable function for user inputs
    - Possibly a modular validation function, or possibly 2 separate ones:
        - validateInt(), and validateOperator() etc
    - We need a series of loops to continually get user input and validate it
    - We will need a reusable function for operations
        - this will need a switch statement for switching between operations
    - We should add some asserts and refactor when done.
*/

/* Notes on stoi():
 * - take a string {foo};
 * - use a size_t {pos} to track the position of the check
 * --> run it through stoi(foo)
 * ---> it parses each character (increasing pos), as long as they are valid ints
 * ---> if it runs into something that's not an int, it stops parsing (pos does not increase)
 * ---> at the end, we compare the length of the string ({foo} == 3) to pos (also 3)
 * -----> pos is kept accurate because it is set to the index right after the last char.
 * ----> if they are the same, it means that every char was an int.
 *
 *
 *
*/

optional<int> validateInteger(const string& originalInput) {
   try {
    // pos is used to track where we are in the char checking in stoi()
    size_t pos;
    int result = stoi(originalInput, &pos);

    // If we got through the whole thing without issues, return true, else false.
    if (pos != originalInput.length()) return nullopt;
    return result;
   }
    // If no valid number could be read
    catch (invalid_argument&) {
        return nullopt;
   }
    // If the int was too big
    catch (out_of_range&) {
        return nullopt;
    }
}

string makeUpper(string stringToConvert) {
    for (char &c : stringToConvert) {
        // Cast it to unsigned char first to avoid negative value issues (toupper expects this)
        const int upper = toupper(static_cast<unsigned char>(c));
        c = static_cast<char>(upper); // Convert each character to uppercase (back to a normal char)
    }

    return stringToConvert;
}

int getNumberFromUser(const string& inputMessage) {

    while (true) {
        string input;
        cout << inputMessage;
        cin >> input;

        const optional<int> validInteger = validateInteger(input);

        if (!validInteger) {
            cout << "Not a valid number, try again.\n";
            continue;
        }

        return validInteger.value();
    }
}

optional<string> validateYesNoAnswer(string answer) {
    answer = makeUpper(answer);
    string allowedValues[4] = {"Y", "N", "YES", "NO"};

    // Range-based for loop --> nice way to iterate over arrays, vectors
    for (const string& operation : allowedValues) {
        if (answer == operation) return answer;
    }

    return nullopt;
}

optional<string> validateOperation(string userOperation) {
    // if there is 1 character, and it is not in the list, return false
    // otherwise return the string of the operator.

    // Should only be one of several values: ["+", "-", "/", "*"]
    string allowedValues[4] = {"+", "-", "/", "*"};

    // If there are more than 1 character, return false
    if (userOperation.length() > 1) return nullopt;

    // Range-based for loop --> nice way to iterate over arrays, vectors
    for (const string& operation : allowedValues) {
        if (userOperation == operation) return userOperation;
    }

    return nullopt;
}

string getOperationFromUser() {
    while (true) {
        string operationInput;
        cout << "Enter the operation to use: \n (+): Addition  \n (-): Subtraction \n (*): Multiplication \n (/): Division \n\n";
        cin >> operationInput;

        const optional<string> validOperation = validateOperation(operationInput);

        if (!validOperation) {
            cout << "Not a valid operation, try again";
            continue;
        }

        return validOperation.value();
    }
}

optional<int> performOperation(const int numberOne, const int numberTwo, const string& chosenOperator) {
    switch (chosenOperator[0]) {
        case '+':
            return numberOne + numberTwo;
        case '-':
            return numberOne - numberTwo;
        case '*':
            return numberOne * numberTwo;
        case '/':
            if (numberTwo == 0) return nullopt;
            return numberOne / numberTwo;
        default:
            cerr << "Error: Unexpected operator '" << chosenOperator[0] << "'\n";
            return nullopt;
    }
}

void calculatorLoop() {
    while (true) {
        const int firstValidatedOperand = getNumberFromUser("Enter your first number: ");
        const string validOperation = getOperationFromUser();
        const int secondValidatedOperand = getNumberFromUser("Enter your second number: ");
        const auto validResult = performOperation(firstValidatedOperand, secondValidatedOperand, validOperation);

        if (validResult) {
            const int actualValue = validResult.value();
            cout << "The result of " << firstValidatedOperand << " " << validOperation << " " << secondValidatedOperand << " is " << actualValue << ".\n";
        } else {
            cout << "Invalid operation (e.g., division by zero).\n\n";
            continue;
        }

        // See if user wants to play again
        while (true) {
            string continueAnswer;
            cout << "Would you like to perform another calculation? (Y/N): \n";
            cin >> continueAnswer;

            optional<string> validAnswer = validateYesNoAnswer(continueAnswer);

            if (!validAnswer) {
                cout << "Invalid, please enter Y or N.\n ";
                continue;
            }

            if (validAnswer == "Y" || validAnswer == "YES") {
                break;
            }

            cout << "Goodbye!";
            return;
        }
    }
}

int main() {
    calculatorLoop();
    return 0;
}