# Snake-in-C
Snake game crafted with C and Ncurses library.

"WASD" to move

To compile and run:
```
gcc snake.c -o snake -I"path-to-curses-library" -lncurses
./snake
```

For error handling:
```
gcc -Wall -g snake.c -o snake I"path-to-curses-library" -lncurses
./snake
```

Works with Powershell version-7.3.6, gcc compiler, ensure Curses library is installed and reachable.

<img width="241" alt="snakec" src="https://github.com/MarcTy/Snake-in-C/assets/88467549/f2f18c9b-db11-42e5-9ac0-ac9a5068847c">

12*12 Dimensions

Buglist:
- Microsoft Powershell version-5.1.22621.1778 crash on fruit pickup
