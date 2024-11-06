<!--
SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>

SPDX-License-Identifier: MIT
-->

# Requite

Requite is going to be a statically typed systems programming langauge with a homoiconic syntax. In this repository, I am experimenting with different compiler design techniques that I need to master before I can put it together.

## Prototype Versions

My current goal is to experiment with compiler design. To do this, I am creating a number of prototypes. These prototypes follow the following rules:

* Easy to debug - I am writting things in a way that are as easy as possible for me to examine using debugging tools.
* Simplification - I am going to support as few language features as possible to make it faster to iterate.
* No Optimizations - I am probably going to rewrite everything several times in the near future. There is no point in optimizing this early. "Premature optimization is the root of all evil." - Donald Knuth

Here is a list of the prototypes

* Homotonic experiment (DONE) - Create a minimal compiler made using LLVM. This is in its own repository: [https://github.com/Journeyman1337/homoiconic_experiment](https://github.com/Journeyman1337/homoiconic_experiment).
* Prototype 1 (DONE) - Generate LLVM IR and compile it. Experiment with different features of LLVM for IR generation. Look at version tag v0.0.1.0 of this repository.
* Prototype 2 (WIP) - Experiment with LLVM static analysis tools.
* Prototype 3 (NOT STARTED) - Create an internal representation of my compiled AST that is in SSA form and perform my own optimizations.
* ???



