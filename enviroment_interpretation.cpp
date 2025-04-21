#include "parsing.cpp"

// 4. Runtime Environment
// The environment manages variable scopes and their values.

#ifndef ENVIROMENT_INTERPRETATION
#define ENVIROMENT_INTERPRETATION
class Environment {
public:
    Environment() : enclosing(nullptr) {}
    Environment(Environment* enclosing) : enclosing(enclosing) {}

    void define(const std::string& name, double value) {
        values[name] = value;
    }

    void assign(const Token& name, double value) {
        // TODO: Step 1 - Check if the variable exists in the current scope
        if (values.find(name.lexeme) != values.end()) {
            // TODO: Step 2 - If found, assign the new value 
            //complete code here
        }
    
        // TODO: Step 3 - If not found, check the enclosing (outer) environment
        if (enclosing != nullptr) {
            // Recursively try in outer scope
            //complete code here
        }
    
        // TODO: Step 4 - If the variable is not found in any scope, throw an error
        throw std::runtime_error("Undefined variable '" + name.lexeme +
                                 "' at line " + std::to_string(name.line));
    }

    double get(const Token& name) {
        // TODO: Step 1 - Look in the current environment
        if (values.find(name.lexeme) != /*complere code here*/) {
            return values[name.lexeme];
        }
    
        // TODO: Step 2 - If not found, and there is an enclosing environment, search there
        if (/*complete code here*/) {
            return enclosing->get(name); // Recursively search outer scope
        }
    
        // TODO: Step 3 - If not found in any scope, throw an error
        throw std::runtime_error("Undefined variable '" + name.lexeme +
                                 "' at line " + std::to_string(name.line));
    }
    

private:
    std::unordered_map<std::string, double> values;
    Environment* enclosing;
};

// 5. Interpretation (Interpreter)
// The interpreter walks the AST, evaluates expressions, and executes statements.

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
    Interpreter() : environment(&global) {}

    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
        try {
            for (const auto& stmt : statements) {
                execute(*stmt);
            }
        } catch (const std::runtime_error& error) {
            std::cerr << "Runtime error: " << error.what() << "\n";
        }
    }

private:
    Environment global;
    Environment* environment;

    void execute(Stmt& stmt) {
        stmt.accept(*this);
    }

    double evaluate(Expr& expr) {
        expr.accept(*this);
        double value = valueStack.back();
        valueStack.pop_back();
        return value;
    }
    double evaluate(Expr& expr) {
        // TODO: Step 1 - Evaluate the expression using the Visitor Pattern
        expr.accept(*this);  // This dispatches to the correct visit method
    
        // TODO: Step 2 - Retrieve the result of the evaluation from the value stack
       /*Complete code here*/ 
    
        // TODO: Step 3 - Remove the value from the stack to maintain stack discipline
        /*Complete code here*/ 
    
        // TODO: Step 4 - Return the final value
        return value;
    }
    

    // ExprVisitor implementations
    void visitLiteralExpr(LiteralExpr& expr) override {
        // TODO: Step 1 - Push the literal's numeric value onto the value stack
        /*Complete the code here*/
    }
    

    void visitVariableExpr(VariableExpr& expr) override {
        // TODO: Step 1 - Push the literal's name value onto the value stack
        double value = environment->get(expr.name);
        /*Complete the code here */
    }


    void visitUnaryExpr(UnaryExpr& expr) override {
        // TODO: Step 1 - Evaluate the operand on the right
        double right = /*complete the code here */;
    
        // TODO: Step 2 - Apply the correct operation based on the operator type
        switch (expr.op.type) {
            case /*complete the code here: Use operation MINUS*/:
                // Negation: push -right
                valueStack.push_back(/*complete the code here*/);
                break;
            case /*complete the code here: Use operation BANG*/:
                // Logical NOT: true if right is 0, false otherwise
                /*complete the code here*/;
                break;
            default:
                // TODO: Step 3 - Report an error if an invalid operator is used
                throw std::runtime_error("Unknown unary operator at line " + std::to_string(expr.op.line));
        }
    }
    

    void visitBinaryExpr(BinaryExpr& expr) override {
        // TODO: Step 1 - Evaluate left and right operands
        double left = evaluate(*expr.left);
        double right = evaluate(*expr.right);
    
        // TODO: Step 2 - Handle binary operations
        switch (expr.op.type) {
            case /*complete the code here: Use operation PLUS*/:
                valueStack.push_back(left + right);
                break;
            case TokenType::MINUS:
                /*complete the code here: Use operation PLUS*/;
                break;
            case /*complete the code here: Use operation STAR*/:
                /*complete the code here*/
                break;
            case TokenType::SLASH:
                if (right == 0) {
                    throw std::runtime_error("Division by zero at line " + std::to_string(expr.op.line));
                }
                valueStack.push_back(left / right);
                break;
    
            // TODO: Step 3 - Handle comparison operators
            case TokenType::GREATER:
                valueStack.push_back(left > right ? 1.0 : 0.0);
                break;
            case /*complete the code here: Use operation GREATER_EQUA*/:
                
                break;
            case TokenType::LESS:
                valueStack.push_back(left < right ? 1.0 : 0.0);
                break;
            case /*complete the code here: Use operation LESS_EQUAL*/:
                
                break;
    
            // TODO: Step 4 - Handle equality operators
            case /*complete the code here: Use operation EQUAL_EQUAL*/:
                valueStack.push_back(isEqual(left, right) ? 1.0 : 0.0);
                break;
            case /*complete the code here: Use operation BANG_EQUAL*/:
                
                break;
    
            // TODO: Step 5 - Default case: unknown operator
            default:
                throw std::runtime_error("Unknown binary operator at line " + std::to_string(expr.op.line));
        }
    }
    

    void visitAssignmentExpr(AssignmentExpr& expr) override {
        // TODO: Step 1 - Evaluate the right-hand side expression
        double value = /*complete the code here: Evaluate the right-hand side expression*/;
    
        // TODO: Step 2 - Assign the value to the variable in the current or enclosing environment
        environment->assign(expr.name, value);
    
        // TODO: Step 3 - Push the assigned value to the value stack (to support nested expressions)
        /*complete the code here: Push the assigned value to the value stack*/
        
    }
    

    // StmtVisitor implementations
    void visitExpressionStmt(ExpressionStmt& stmt) override {
        evaluate(*stmt.expression);
    }

    void visitAssignmentExpr(AssignmentExpr& expr) override {
        // TODO: Step 1 - Evaluate the right-hand side expression
        double value = evaluate(*expr.value);
    
        // TODO: Step 2 - Assign the value to the variable in the current or enclosing environment
        environment->assign(expr.name, value);
    
        // TODO: Step 3 - Push the assigned value to the value stack (to support nested expressions)
        valueStack.push_back(value);
    }
    

    void visitBlockStmt(BlockStmt& stmt) override {
        // TODO: Step 1 - Create a new environment that encloses the current one
        ///*complete the code here: create a newEnv object, pass environment as argument*/
        
    
        // TODO: Step 2 - Save the current environment so we can restore it later
        Environment* previous = environment;
    
        // TODO: Step 3 - Use a try-catch block to ensure the original environment is restored
        environment = &newEnv;
        try {
            // TODO: Step 4 - Execute all statements in the block
            for (const auto& statement : stmt.statements) {
                /*complete the code here: use the execute function that will recevive statement value*/
               
            }
        } catch (...) {
            // On exception, restore environment before re-throwing
            environment = previous;
            throw;
        }
    
        // TODO: Step 5 - Restore the outer environment
        environment = previous;
    }
    

    void visitIfStmt(IfStmt& stmt) override {
        double condition = evaluate(*stmt.condition);
        if (isTruthy(condition)) {
            execute(*stmt.thenBranch);
        } else if (stmt.elseBranch) {
            execute(*stmt.elseBranch);
        }
    }

    void visitWhileStmt(WhileStmt& stmt) override {
        while (isTruthy(evaluate(*stmt.condition))) {
            execute(*stmt.body);
        }
    }

    void visitPrintStmt(PrintStmt& stmt) override {
        double value = evaluate(*stmt.expression);
        std::cout << value << "\n";
    }

    // Helper functions
    bool isTruthy(double value) {
        return value != 0.0;
    }

    bool isEqual(double a, double b) {
        return a == b;
    }

    std::vector<double> valueStack;
};
#endif