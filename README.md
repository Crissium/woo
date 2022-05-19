# Description

This is a crudely made two-player Connect-5 (also known as five-in-a-row, or Gomoko in Japanese) game with alpha-beta pruning.

Get it from Open Build Service: [woo](https://software.opensuse.org/package/woo).

# Controls

- Z: undo last two moves
- R: restart game
- A: Let AI make a move for you
- Num 1-6: set AI search depth

# Build

```
$ make
# make install # install with this command
```

This game depends on SFML 2.5 and you need a C++17-compliant compiler to build.

Note: The paths of the font file and images used for rendering is hard-coded.

# Warning

This is written for my C++ coursework assignment. Do not simply clone and pass it off as your own!
