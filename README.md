# LISPish

## A small `Lisp-ish' programming language, written in C

"Any sufficiently complicated C or Fortran program contains an ad hoc,
informally-specified, bug-ridden, slow implementation of half of
Common Lisp."
[*Greenspun's Tenth Rule*](http://en.wikipedia.org/wiki/Greenspun's_tenth_rule)

Dependencies:
 - The editline library (Linux & Mac)

Includes:
 - [*MPC*](https://github.com/orangeduck/mpc), a parser combinator library

Based on:
 - [*Build Your Own Lisp*](http://www.buildyourownlisp.com/)

To install:
 - 'make evaluation' from project root
 - './evaluation' to run

Todo:
 - floating point numbers
 - negation

Sample session:
    "Oh! moon old boughs LISP forth a holier din" (Keats)
    Lispish Version 0.0.0.0.3
    Press Ctrl-c to Exit

    lispish> + 2 3
    + 2 3
    5
    lispish> * 3 (min 7 8 9) (/ 12 3) (- 4 (% 5 2))
    * 3 (min 7 8 9) (/ 12 3) (- 4 (% 5 2))
    252
    lispish>
