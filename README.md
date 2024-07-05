# Algorithms-CS
Algorithms language C
binary tree, queue, list, chained list...

Gdb ./expr_ex1
Break 
Run ../test/..........

1. "break computeExpressions": You have already defined a breakpoint inside the "computeExpressions" function with this command.
2. continue": Continue executing your program until it reaches the breakpoint.
3. step': Execute one instruction at a time. This will allow you to follow the execution flow step by step in the "computeExpressions" function.
4. next': Execute the current line, then stop at the next line. This is useful if you don't want to enter the functions called by computeExpressions'.
5. print variable': Use this command to display the value of a specific variable at a given time. For example, print linep will display the value of linep*.
6. info locals": Displays local variables in the current scope.
7.    ***backtrace" (or simply bt*): Displays the call stack, showing where you are in the code.
8.    **finish': Executes the program to the end of the current function, then stops.
LO. Repeat steps 4 to 9 to move forward in the code and inspect variables.
11. ** quit': Exits GDB when finished. The exact order will depend on what you wish to achieve during debugging. You can choose to run step' or "next" to follow the execution flow of your program step by step, then use 'print' and 'info locals to check local variable values at each step. Use "backtrace" to understand where you are in the call stack. Finally, use "finish" to exit a function and return to its appera.



Gdb ./expr_ex1
Break 
Run ../test/……….

1.    "break computeExpressions" : Vous avez déjà défini un point d'arrêt à l'intérieur de la fonction "computeExpressions" avec cette commande.
2.    continue" : Continuez l'exécution de votre programme jusqu'à ce qu'il atteigne le point d'arrêt.
3.    step' : Exécutez une seule instruction à la fois. Cela vous permettra de suivre pas à pas le flux d'exécution dans la fonction "computeExpressions.
4.    next' : Exécutez la ligne actuelle, puis s'arrête à la ligne suivante. Cela est utile si vous ne voulez pas entrer dans les fonctions appelées par computeExpressions'
5.    print variable' : Utilisez cette commande pour afficher la valeur d'une variable spécifique à un moment donné. Par exemple, print linep affichera la valeur de linep*
6.    info locals" : Pour afficher les variables locales dans la portée actuelle.
7.    ***backtrace" (ou simplement bt*): Affiche la pile d'appels, montrant où vous êtes dans le code.
8.    **finish' : Exécute le programme jusqu'à la fin de la fonction actuelle, puis s'arrête.
LO. Répétez les étapes 4 à 9 pour avancer dans le code et inspecter les variables.
11. ** quit' : Pour quitter GDB lorsque vous avez terminé. L'ordre exact dépendra de ce que vous souhaitez accomplir pendant le débogage. Vous pouvez choisir d'exécuter step' ou "next" pour suivre le flux d'exécution de votre programme pas à pas, puis utiliser 'print' et 'info locals pour vérifier les valeurs des variables locales à chaque étape. Utilisez "backtrace" pour comprendre où vous ête la pile d'appels. Enfin, utilisez "finish" pour sortir d'une fonction et revenir à son appera.
