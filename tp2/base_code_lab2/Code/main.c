#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "token.h"
#include "queue.h"
#include "stack.h"


bool isSymbol(char c) {
    return (c == '+' || c == '-' || c == '/' || c == '*' || c == '^' || c == '(' || c == ')');
}

float evaluateOperator(Token *arg1, Token *op, Token *arg2) {
    switch (tokenGetOperatorSymbol(op)) { // switch pour voir si op est + - ect.. le return se fait en fonction du op
        case '+':
            return tokenGetValue(arg1) + tokenGetValue(arg2);
        case '-':
            return tokenGetValue(arg1) - tokenGetValue(arg2);
        case '*':
            return tokenGetValue(arg1) * tokenGetValue(arg2);
        case '/':
            return tokenGetValue(arg1) / tokenGetValue(arg2);
        case '^':
            return pow(tokenGetValue(arg1), tokenGetValue(arg2));
        default:
            printf("erreur pas dans les cases du switch ");
            exit(1);
    }
}

float evaluateExpression(Queue *postfix) {
    Stack *stack = createStack(35);
    Queue *queoperand = createQueue();

    while (!queueEmpty(postfix)) {
        Token *currentToken = (Token *)queueTop(postfix);

        if (tokenIsOperator(currentToken)) {
            Token *operand2 = (Token *)stackTop(stack);
            stack = stackPop(stack);
            Token *operand1 = (Token *)stackTop(stack);
            stack = stackPop(stack);
            float result = evaluateOperator(operand1, currentToken, operand2);
            queoperand = queuePush(queoperand,currentToken); // ----------------------------
            Token *resultToken = createTokenFromValue(result);
            stack = stackPush(stack, resultToken); 

            // Libérer la mémoire des opérandes
            deleteToken(&operand1);
            deleteToken(&operand2);
            //deleteToken(&resultToken);

        } else if (tokenIsNumber(currentToken)) {
            stack = stackPush(stack, currentToken);

        }

        postfix = queuePop(postfix);
    }

    Token *finalResultToken = (Token *)stackTop(stack);
    float finalResult = tokenGetValue(finalResultToken);
    deleteToken(&finalResultToken);

    // Libérer la mémoire des opérandes stockées dans la file d'attente
    while (!queueEmpty(queoperand)) {
        Token *operand = (Token *)queueTop(queoperand);
        deleteToken(&operand);
        queoperand = queuePop(queoperand);
    }

    // Libération de la mémoire de la file d'attente
    deleteQueue(&queoperand);
    deleteStack(&stack);

    return finalResult;
}

Queue *shuntingYard(Queue *infix) {
    Stack *operatorStack = createStack(35);
    Queue *outputQueue = createQueue();
    Queue *parenthesisTokens = createQueue();

    while (!queueEmpty(infix)) {
        if (tokenIsNumber(queueTop(infix))) {
            outputQueue = queuePush(outputQueue, queueTop(infix)); //push dans outQueue ----------------------
        }

        else if (tokenIsOperator(queueTop(infix))) { //21/10 rajout du else
            while (!stackEmpty(operatorStack) && tokenIsOperator(stackTop(operatorStack)) && ((tokenGetOperatorPriority(stackTop(operatorStack)) >= tokenGetOperatorPriority(queueTop(infix))) || ((tokenGetOperatorPriority(stackTop(operatorStack)) >= tokenGetOperatorPriority(queueTop(infix))) && tokenOperatorIsLeftAssociative(queueTop(infix))))){
                outputQueue = queuePush(outputQueue, stackTop(operatorStack));//push dans outQueue ----------------------
                operatorStack = stackPop(operatorStack);
            }
            operatorStack = stackPush(operatorStack, queueTop(infix));
        }

        else if (tokenGetParenthesisSymbol(queueTop(infix)) == '(') { //21/10 else ici
            operatorStack = stackPush(operatorStack, queueTop(infix));//push dans stack de token dans Qinfix
            parenthesisTokens = queuePush(parenthesisTokens,queueTop(infix));
        }

        else if (tokenGetParenthesisSymbol(queueTop(infix)) == ')') {//21/10 rajout du else
            while (!stackEmpty(operatorStack) && tokenIsOperator(stackTop(operatorStack))) {  //changement 21/10
                outputQueue = queuePush(outputQueue, stackTop(operatorStack)); //push dans outQueue ----------------------
                operatorStack = stackPop(operatorStack);
            }
            operatorStack =stackPop(operatorStack);
            parenthesisTokens = queuePush(parenthesisTokens,queueTop(infix));
        }
        infix = queuePop(infix);
    }

    while (!stackEmpty(operatorStack)) {
        outputQueue = queuePush(outputQueue, stackTop(operatorStack)); //push dans outQueue ----------------------
        operatorStack = stackPop(operatorStack);
    }
    deleteStack(&operatorStack);


    while (!queueEmpty(parenthesisTokens)) {
        Token *parenthesisToken = (Token *)queueTop(parenthesisTokens);
        deleteToken(&parenthesisToken); // Libérez la mémoire pour chaque token de parenthèses
        queuePop(parenthesisTokens);
    }
    deleteQueue(&parenthesisTokens); // Libérez la queue de parenthèses
    return outputQueue;
}

Queue *stringToTokenQueue(const char *expression) { // valgrind indique que le malloc non free est issue de cette fonction?..
    Queue *queue = createQueue();
    const char *curpos = expression;

    while (*curpos != '\0') {
        if (*curpos != ' ' && *curpos != '\n' && *curpos != '\r') {
            Token *token = NULL;
            if (isSymbol(*curpos)) {
                // Obtenez la longueur du symbole
                int symbolLength = 0;
                while (curpos[symbolLength] != '\0' && isSymbol(curpos[symbolLength])) {
                    symbolLength++;
                }
                token = createTokenFromString(curpos, symbolLength);
            } else {
                token = createTokenFromValue(*curpos - '0');
            }

            queue = queuePush(queue, token);

           // deleteToken(&token);
        }
        curpos++;
    }

    return queue;
}

void printToken(FILE *f, const void *e) { 
    Token *T = (Token *)e;
    tokenDump(f, T);

}




void computeExpressions(FILE *input) {
    char *linep = NULL;
    size_t lenlinecap = 0;
    ssize_t read_err;

    while ((read_err = getline(&linep, &lenlinecap, input)) != -1) {
        if (strlen(linep) > 1) {
            printf("Input   : %s", linep);

            Queue *queue = stringToTokenQueue(linep);
            Queue *queue1 = queue;
   
            printf("Infix   :");
            queueDump(stdout, queue, printToken);
            printf("\n");
            Queue *postfixQ = shuntingYard(queue1);//probleme 80 bytes perdu a partir d'ici ???????
            printf("Postfix :");
            queueDump(stdout, postfixQ, printToken);
            printf("\n");
            
           float result = evaluateExpression(postfixQ);
           printf("Evaluate : %f\n \n", result);

           
            // Libérer mémoire pour queue
            while (!queueEmpty(queue)) {   // pas vraiment besoin car PostfixQ le fait pour les memes adresses.
                Token *token = (Token *)queueTop(queue);
                queue = queuePop(queue);
                deleteToken(&token); // Libérer la mémoire pour chaque token
            }

            // Libérer mémoire pour postfixQ
             while (!queueEmpty(postfixQ)) {
                Token *tokenQ = (Token *)queueTop(postfixQ);
                postfixQ = queuePop(postfixQ);
                deleteToken(&tokenQ); // Libérer la mémoire pour chaque token
            }

            deleteQueue(&queue);
            deleteQueue(&postfixQ);
            
        }
    }

    free(linep);
}









int main(int argc, char **argv) {
    FILE *input;
    if (argc < 2) {
        fprintf(stderr, "usage : %s filename\n", argv[0]);
        return 1;
    }

    input = fopen(argv[1], "r");

    if (!input) {
        perror(argv[1]);
        return 1;
    }

    computeExpressions(input);

    fclose(input);
    
    return 0;
}
