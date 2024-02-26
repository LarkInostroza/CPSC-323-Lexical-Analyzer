#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

//Token struct (Will store each token's type and value)
struct Token {
    std::string type;//:3
    std::string value;
};

//Declaring Lexer Function
Token lexer(std::ifstream& inputFile);

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    Token currentToken;
    //fun way to loop the function until the end of file
    while ((currentToken = lexer(inputFile)).type != "EOF") {
        outputFile << "Token: " << currentToken.type << "\tLexeme: " << currentToken.value << std::endl;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}

//The world's worst switch statement called a lexer
Token lexer(std::ifstream& inputFile) {
    enum class State { START, IN_IDENTIFIER, IN_INTEGER, IN_REAL, IN_OPERATOR, DONE };

    State currentState = State::START;
    std::string lexeme;
    char currentChar;

    while (inputFile.get(currentChar)) {
        switch (currentState) {
            case State::START:
                lexeme.clear();
                if (std::isalpha(currentChar)) {
                    currentState = State::IN_IDENTIFIER;
                    lexeme += currentChar;
                } 
                else if (std::isdigit(currentChar)) {
                    currentState = State::IN_INTEGER;
                    lexeme += currentChar;
                } 
                else if (std::isspace(currentChar)) {
                    // Skip whitespace
                } 
                else if (currentChar == '(' || currentChar == ')' || currentChar == ';') {
                    lexeme += currentChar;
                    return { "Separator", lexeme };
                } 
                else if (currentChar == '=') {
                    lexeme += currentChar;
                    return { "Operator", lexeme };
                } 
                else {
                    currentState = State::IN_OPERATOR;
                    lexeme += currentChar;
                }
                break;
            case State::IN_IDENTIFIER:
                if (std::isalnum(currentChar)) {
                    lexeme += currentChar;
                } 
                else {
                    inputFile.unget(); // Return currentChar to stream
                    if (lexeme == "while" || lexeme == "if" || lexeme == "else") {
                        return { "Keyword", lexeme };
                    }
                    return { "Identifier", lexeme };
                }
                break;
            case State::IN_INTEGER:
                if (std::isdigit(currentChar)) {
                    lexeme += currentChar;
                } 
                else if (currentChar == '.') {
                    currentState = State::IN_REAL;
                    lexeme += currentChar;
                } 
                else {
                    inputFile.unget(); // Return currentChar to stream
                    return { "Integer", lexeme };
                }
                break;
            case State::IN_REAL:
                if (std::isdigit(currentChar)) {
                    lexeme += currentChar;
                } 
                else {
                    inputFile.unget(); // Return currentChar to stream
                    return { "Real", lexeme };
                }
                break;
            case State::IN_OPERATOR:
                // Handle other operators
                lexeme += currentChar;
                return { "Operator", lexeme };
            case State::DONE:
                break;//:3
        }
    }

    // End of file reached
    return { "EOF", "" };
}