quiver
======
[![Build Status](https://travis-ci.org/gciruelos/quiver.svg?branch=master)](https://travis-ci.org/gciruelos/quiver)


Quiver is a graph-based esoteric programming language.

It was designed as an entry for [CALESYTA 2016](http://calesyta.xyz/en/).

Syntax
------

Programs in Quiver are directed graphs (digraphs), where

* Vertices are conditions, and
* Edges are side effects.

The execution will flow through that graph, jumping from vertex to vertex. Each vertex has two outgoing edges, one will be followed if the condition in the vertex is true, the other if the condition is false.

A program in Quiver is a list of nodes. Each node has 

1. A node number,
1. A condition,
2. A node to jump to if the condition is true,
3. An action to perform if the condition is true,
4. A node to jump to if the condition is false,
5. An action to perform if the condition is false,

There are two distinguished edges: the starting vertex and the 'halt' vertex. The first one is the one where the program starts, and the other one is where the program ends (if the programs arrives to it, it halts). The starting vertex will be the vertex with the smallest number.

The syntax for a node is

```
<number>?<condition>?<node_if_true>(<action_if_true>)<node_if_false>(<action_if_false>)
```

To specify that you want to go to the 'halt' vertex, you should leave `node_if_[true|false]` empty.
  
A program in Quiver is just a list of nodes. Let's see an example.

```
0??('Hello, World!\n)()
```

That is "Hello, World!" in Quiver. It starts in 0. The empty condition evaluates to True, so it jumps to the halt node, writing "Hello, World!" to the screen. Note that there's only one `'`.

State
-----

There's also state in Quiver.
There's a global accumulator that you can always see and modify,
and also each node has a value that you can see and modify only when you are in that node.

All values start in 0. The starting value of the accumulator can be set passing it as a parameter.

Let's see an example of that being used, a program that outputs the numbers from 1 to 10.

```
0??1(1)()
1?<11?2(p)()
2??3('\n)()
3??1(++)()
```

Let's see what happens:

1. It starts in node 0 and jumps to 1. The action `(1)` means just assign 1 to the accumulator.

2. In node 1, if the accumulator is greater than or equal to 11, it halts. If it is less, it jumps to 2, and you print it (that is what `p` does, it prints the accumulator).

3. It prints `\n` and jumps to 3.

4. It jumps to 1 and increments the accumulator.

![1to10.quiv Graph](doc/img/1to10.png)

Here you can clearly see what's going on. Green edges mean true branch, while red edges mean false branch. The blue box is the starting node.

This graph was generated automatically, we will learn later how.

More
----

You can read more about Quiver in the [reference manual](doc/reference.md).

How to use
----------

To clone the repository and compile everything:

    git clone https://github.com/gciruelos/quiver.git
    cd quiver
    make

To see everything you can do, just run

    ./quiver


Then run some examples and write some of your own.

    ./quiver examples/primes.quiv
    ./quiver examples/hangs.quiv

If you want to generate the graph of a given program, run

    ./quiver --dot examples/<program>.quiv > graph.dot

Which will generate a file called `graph.dot`, which you can transform to an image using the program `dot`.

    dot -Tpng graph.dot -o graph.png


Implementation
--------------

  I implemented a C++ interpreter for the language. I'd also like to write a LLVM backend if I have the time.

Contributing
------------

Feel free to open an issue for suggestions, bugs, ideas, etc. Feel free to make pull requests, too.
