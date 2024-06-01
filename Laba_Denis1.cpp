#include <iostream>
#include <vector>
#include <stdexcept>

class BinaryNumber {
private:
    std::vector<bool> bits;
    int size;

    void convertToBinary(int decimalNumber) {
        bool isNegative = decimalNumber < 0;
        if (isNegative) {
            decimalNumber = -decimalNumber;
        }

        bits.resize(size, false);
        for (int i = 0; i < size; ++i) {
            bits[size - 1 - i] = (decimalNumber >> i) & 1;
        }

        if (isNegative) {
            // Применяем дополнительный код для отрицательных чисел
            for (int i = 0; i < size; ++i) {
                bits[i] = !bits[i];
            }
            for (int i = size - 1; i >= 0; --i) {
                if (!bits[i]) {
                    bits[i] = true;
                    break;
                }
                bits[i] = false;
            }
        }
    }

    int convertToDecimal() const {
        int decimalNumber = 0;
        bool isNegative = bits[0];

        if (isNegative) {
            // Инвертируем биты и добавляем 1 для отрицательных чисел
            for (int i = 1; i < size; ++i) {
                decimalNumber |= (!bits[i]) << (size - 1 - i);
            }
            decimalNumber = -(decimalNumber + 1);
        }
        else {
            for (int i = 1; i < size; ++i) {
                decimalNumber |= bits[i] << (size - 1 - i);
            }
        }

        return decimalNumber;
    }


public:
    BinaryNumber(int decimalNumber, int size) : size(size) {
        if (size != 8 && size != 16 && size != 32) {
            throw std::invalid_argument("Invalid binary number size. Allowed sizes: 8, 16, 32.");
        }
        convertToBinary(decimalNumber);
    }

    std::string getBinaryString() const {
        std::string binaryString;
        for (int i = 0; i < size; ++i) {
            binaryString += bits[i] ? '1' : '0'; //если true добовляем 1, если false 0
        }
        return binaryString;
    }

    int getDecimalNumber() const {
        return convertToDecimal();
    }

    BinaryNumber operator+(const BinaryNumber& other) const {

        int carry = 0;
        std::vector<bool> resultBits(size, false);

        for (int i = size - 1; i >= 0; --i) {
            int sum = bits[i] + other.bits[i] + carry;
            resultBits[i] = sum & 1;
            carry = sum >> 1;
        }

        if (carry) {
            throw std::overflow_error("Overflow occurred during addition.");
        }

        BinaryNumber result(0, size);
        result.bits = resultBits;
        return result;
    }

    BinaryNumber operator-(const BinaryNumber& other) const {

        BinaryNumber result(0, size);
        bool borrow = false;

        for (int i = size - 1; i >= 0; --i) {
            bool diff = bits[i] ^ other.bits[i] ^ borrow;
            borrow = (!bits[i] && borrow) || (!bits[i] && other.bits[i]) || (other.bits[i] && borrow);
            result.bits[i] = diff;
        }

        return result;
    }


    BinaryNumber operator*(const BinaryNumber& other) const {
        BinaryNumber result(0, size);
        BinaryNumber shifted = *this;

        for (int i = size - 1; i >= 0; --i) {
            if (other.bits[i]) {
                result = result + shifted;
            }
            shifted = shiftLeft(shifted);
        }

        if (result.bits[0] != bits[0] && result.bits[0] != other.bits[0]) {
            throw std::overflow_error("Overflow occurred during multiplication.");
        }

        return result;
    }

    BinaryNumber shiftLeft(const BinaryNumber& num) const {
        BinaryNumber shifted = num;
        for (int i = 0; i < size - 1; ++i) {
            shifted.bits[i] = shifted.bits[i + 1];
        }
        shifted.bits[size - 1] = false;
        return shifted;
    }



};

class Calculator {
private:
    BinaryNumber num1;
    BinaryNumber num2;
    char operation;

public:
    Calculator(int decimalNum1, int decimalNum2, int size, char operation)
        : num1(decimalNum1, size), num2(decimalNum2, size), operation(operation) {}

    BinaryNumber calculate() {
        switch (operation) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        default:
            throw std::invalid_argument("Invalid operation. Allowed operations: +, -, *.");
        }
    }

    void printResult() {
        BinaryNumber result = calculate();
        std::cout << "Binary Result: " << result.getBinaryString() << std::endl;
        std::cout << "Decimal Result: " << result.getDecimalNumber() << std::endl;
    }
};

int main() {
    int decimalNum1, decimalNum2, size;
    char operation;

    std::cout << "Enter the binary number size (8, 16, or 32): ";
    std::cin >> size;

    std::cout << "Enter the first decimal number: ";
    std::cin >> decimalNum1;

    std::cout << "Enter the second decimal number: ";
    std::cin >> decimalNum2;

    std::cout << "Enter the operation (+, -, *): ";
    std::cin >> operation;

    try {
        Calculator calculator(decimalNum1, decimalNum2, size, operation);
        calculator.printResult();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
