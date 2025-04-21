// g++ -std=c++17 -o cameleon main.cpp 

#include "enviroment_interpretation.cpp"

int main() {
    std::string source = R"(
        let x = 5;
        let y = 10;
        if (x < y) {
            x = x + 1;
        } else {
            y = y + 1;
        }
        while (x < 10) {
            x = x + 1;
            print x;
        }
        print y;
    )";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    Parser parser(tokens);
    auto statements = parser.parse();

    Interpreter interpreter;
    interpreter.interpret(statements);

    return 0;
}