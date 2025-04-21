
#include "abstract_syntax_tree.cpp"
/*3. Parsing (Parser)
The parser consumes tokens and produces the AST.*/

#ifndef PARSING
#define PARSING

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    std::vector<std::unique_ptr<Stmt>> parse() {
        std::vector<std::unique_ptr<Stmt>> statements;
        while (!isAtEnd()) {
            auto stmt = declaration();
            if (stmt) {
                statements.push_back(std::move(stmt));
            }
        }
        return statements;
    }

private:
    const std::vector<Token>& tokens;
    size_t current = 0;

    bool isAtEnd() {
        return peek().type == TokenType::END_OF_FILE;
    }

    const Token& peek() {
        return tokens[current];
    }

    const Token& previous() {
        return tokens[current - 1];
    }

    const Token& advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    bool match(const std::vector<TokenType>& types) {
        for (const auto& type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    const Token& consume(TokenType type, const std::string& message) {
        if (check(type)) return advance();

        throw std::runtime_error("Parser error at line " + std::to_string(peek().line) + ": " + message);
    }

    // Declarations
    std::unique_ptr<Stmt> declaration() {
        try {
            if (match({TokenType::LET})) {
                return variableDeclaration();
            }
            if (match({TokenType::PRINT})) {
                return printStatement();
            }
            return statement();
        } catch (const std::runtime_error& error) {
            synchronize();
            return nullptr;
        }
    }

    std::unique_ptr<Stmt> variableDeclaration() {
        const Token& name = consume(TokenType::IDENTIFIER, "Expect variable name.");

        std::unique_ptr<Expr> initializer;
        if (match({TokenType::EQUAL})) {
            initializer = expression();
        }

        consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
        return std::make_unique<VariableDeclarationStmt>(name, std::move(initializer));
    }

    // Statements
    std::unique_ptr<Stmt> statement() {
        if (match({TokenType::IF})) {
            return ifStatement();
        }
        if (match({TokenType::WHILE})) {
            return whileStatement();
        }
        if (match({TokenType::LEFT_BRACE})) {
            return std::make_unique<BlockStmt>(block());
        }
        return expressionStatement();
    }

    std::unique_ptr<Stmt> printStatement() {
        auto value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_unique<PrintStmt>(std::move(value));
    }

    std::unique_ptr<Stmt> ifStatement() {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
        auto condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

        auto thenBranch = statement();
        std::unique_ptr<Stmt> elseBranch;
        if (match({TokenType::ELSE})) {
            elseBranch = statement();
        }

        return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    }

    std::unique_ptr<Stmt> whileStatement() {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
        auto condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
        auto body = statement();

        return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
    }

    std::vector<std::unique_ptr<Stmt>> block() {
        std::vector<std::unique_ptr<Stmt>> statements;

        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            auto decl = declaration();
            if (decl) {
                statements.push_back(std::move(decl));
            }
        }

        consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }

    std::unique_ptr<Stmt> expressionStatement() {
        auto expr = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after expression.");
        return std::make_unique<ExpressionStmt>(std::move(expr));
    }

    // Expressions
    std::unique_ptr<Expr> expression() {
        return assignment();
    }

    std::unique_ptr<Expr> assignment() {
        auto expr = equality();

        if (match({TokenType::EQUAL})) {
            const Token& equals = previous();
            auto value = assignment();

            if (auto varExpr = dynamic_cast<VariableExpr*>(expr.get())) {
                Token name = varExpr->name;
                return std::make_unique<AssignmentExpr>(name, std::move(value));
            }

            throw std::runtime_error("Invalid assignment target at line " + std::to_string(equals.line));
        }

        return expr;
    }

    std::unique_ptr<Expr> equality() {
        auto expr = comparison();

        while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
            const Token& op = previous();
            auto right = comparison();
            expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> comparison() {
        auto expr = term();

        while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
            const Token& op = previous();
            auto right = term();
            expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> term() {
        auto expr = factor();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            const Token& op = previous();
            auto right = factor();
            expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> factor() {
        auto expr = unary();

        while (match({TokenType::SLASH, TokenType::STAR})) {
            const Token& op = previous();
            auto right = unary();
            expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> unary() {
        if (match({TokenType::BANG, TokenType::MINUS})) {
            const Token& op = previous();
            auto right = unary();
            return std::make_unique<UnaryExpr>(op, std::move(right));
        }

        return primary();
    }

    std::unique_ptr<Expr> primary() {
        if (match({TokenType::FALSE})) return std::make_unique<LiteralExpr>(0.0);
        if (match({TokenType::TRUE})) return std::make_unique<LiteralExpr>(1.0);
        if (match({TokenType::NIL})) return std::make_unique<LiteralExpr>(0.0);

        if (match({TokenType::NUMBER})) {
            double value = std::stod(previous().lexeme);
            return std::make_unique<LiteralExpr>(value);
        }

        if (match({TokenType::IDENTIFIER})) {
            return std::make_unique<VariableExpr>(previous());
        }

        if (match({TokenType::LEFT_PAREN})) {
            auto expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
            return expr;
        }

        throw std::runtime_error("Expect expression at line " + std::to_string(peek().line));
    }

    void synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous().type == TokenType::SEMICOLON) return;

            switch (peek().type) {
                case TokenType::AND:
                case TokenType::OR:
                case TokenType::LET:
                case TokenType::ELSE:
                case TokenType::IF:
                case TokenType::WHILE:
                case TokenType::PRINT:
                case TokenType::FALSE:
                    return;
                default:
                    break;
            }

            advance();
        }
    }
};
#endif