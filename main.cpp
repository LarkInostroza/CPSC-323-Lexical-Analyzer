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
bool isKeyWord(std::string current);
bool isOperator(char current);
bool isSeparator(char current);

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
bool isOperator(char current) {
  return (current == '+') || (current == '-') || (current == '*') || (current == '/') || (current == '%') ||
     (current == '?') || (current == ',') || (current == '&') || (current == '|') ||
     (current == '^') || (current == '~') || (current == '<') || (current == '>');
}
bool isSeparator(char current) {
    const char separators[] = { ';', ',', ':', '.', '-', '>', '(', ')', '[', ']', '{', '}', '#', '\\'};
    for (char separator : separators) {
        if (current == separator) {
            return true;
        }
    }
    return false;
}
bool isKeyword(std::string current) {
    static const std::string keywords[] = {
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
        "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
        "class", "compl", "concept", "const", "consteval", "constexpr", "const_cast",
        "continue", "co_await", "co_return", "co_yield", "decltype", "default",
        "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export",
        "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int",
        "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
        "operator", "or", "or_eq", "private", "protected", "public", "register",
        "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static",
        "static_assert", "static_cast", "struct", "switch", "synchronized", "template",
        "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename",
        "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while",
        "xor", "xor_eq"
    };
    for (const std::string& keyword : keywords) {
        if (current == keyword) {
            return true;
        }
    }
    return false;
}



//The world's worst switch statement called a lexer
Token lexer(std::ifstream& inputFile) {
    enum class State { START, IN_IDENTIFIER, IN_INTEGER, IN_REAL, IN_OPERATOR, IN_COMMENT, IN_STRING, DONE};

    State currentState = State::START;
    std::string lexeme;
    char currentChar;

    while (inputFile.get(currentChar)) {
        switch (currentState) {
            case State::START:
                lexeme.clear();
                if (std::isalpha(currentChar) || currentChar == '_') {
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
                else if (isSeparator(currentChar)) {
                    lexeme += currentChar;
                    return { "Separator", lexeme };
                } 
                else if (currentChar == '/'){
                  currentState = State::IN_COMMENT;
                  lexeme += currentChar;
                }
                else if (isOperator(currentChar)) {
                    lexeme += currentChar;
                    return { "Operator", lexeme };
                } 
                else if (currentChar == '"') {
                    currentState = State::IN_STRING;
                    lexeme += currentChar;
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
                    if (isKeyword(lexeme)) {
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
                    inputFile.unget(); 
                    return { "Integer", lexeme };
                }
                break;
            case State::IN_REAL:
                if (std::isdigit(currentChar)) {
                    lexeme += currentChar;
                } 
                else {
                    inputFile.unget(); //Real as in real number
                    return { "Real", lexeme };
                }
                break;
            case State::IN_OPERATOR:
                
                lexeme += currentChar;
                return { "Operator", lexeme };
            case State::IN_COMMENT:
                if (currentChar == '/') {
                  //Skip characters until newline character or end of file
                  while (inputFile.get(currentChar) && currentChar != '\n') {
                  //Ignore until end
                  }
                currentState = State::START;
                }
                break;
            case State::IN_STRING:
                lexeme += currentChar;
                if (currentChar == '"') {
                  return { "String", lexeme };
                }
                break;
            case State::DONE:
                break;
        }
    }
    // End of file reached
    return { "EOF", "" };
}

