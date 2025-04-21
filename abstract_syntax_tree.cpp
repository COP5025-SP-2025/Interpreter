#include "tokenization.cpp"


/*2. Abstract Syntax Tree (AST)
The AST represents the hierarchical structure of the source code. Each node corresponds to a syntactic construct.*/

#ifndef abstract_syntax
#define abstract_syntax

class ExprVisitor; // Forward declaration

class Expr {
public:
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor& visitor) = 0;
};

class LiteralExpr;
class VariableExpr;
class UnaryExpr;
class BinaryExpr;
class AssignmentExpr;

// Define the visitor interface
class ExprVisitor {
public:
    virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual void visitVariableExpr(VariableExpr& expr) = 0;
    virtual void visitUnaryExpr(UnaryExpr& expr) = 0;
    virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual void visitAssignmentExpr(AssignmentExpr& expr) = 0;
};

// Literal Expression
class LiteralExpr : public Expr {
public:
    double value;

    LiteralExpr(double value) : value(value) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitLiteralExpr(*this);
    }
};

// Variable Expression
class VariableExpr : public Expr {
public:
    Token name;

    VariableExpr(const Token& name) : name(name) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitVariableExpr(*this);
    }
};

// Unary Expression
class UnaryExpr : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    UnaryExpr(const Token& op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitUnaryExpr(*this);
    }
};

// Binary Expression
class BinaryExpr : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, const Token& op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitBinaryExpr(*this);
    }
};

// Assignment Expression
class AssignmentExpr : public Expr {
public:
    Token name;
    std::unique_ptr<Expr> value;

    AssignmentExpr(const Token& name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitAssignmentExpr(*this);
    }
};

//Similarly, we define statement classes:

class StmtVisitor; // Forward declaration

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) = 0;
};

class ExpressionStmt;
class VariableDeclarationStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class PrintStmt;

// Define the visitor interface
class StmtVisitor {
public:
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitVariableDeclarationStmt(VariableDeclarationStmt& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
    virtual void visitPrintStmt(PrintStmt& stmt) = 0;
};

// Expression Statement
class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;

    ExpressionStmt(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitExpressionStmt(*this);
    }
};

// Variable Declaration Statement
class VariableDeclarationStmt : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> initializer;

    VariableDeclarationStmt(const Token& name, std::unique_ptr<Expr> initializer)
        : name(name), initializer(std::move(initializer)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitVariableDeclarationStmt(*this);
    }
};

// Block Statement
class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitBlockStmt(*this);
    }
};

// If Statement
class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitIfStmt(*this);
    }
};

// While Statement
class WhileStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitWhileStmt(*this);
    }
};

// Print Statement
class PrintStmt : public Stmt {
public:
    std::unique_ptr<Expr> expression;

    PrintStmt(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitPrintStmt(*this);
    }
};
#endif