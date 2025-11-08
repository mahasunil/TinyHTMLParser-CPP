# Tiny HTML Parser in c++

A simple HTML parser written in C++ that builds a DOM-like tree and prints nested HTML tags with their text content.

## Features
   - Builds a tree of nodes representing HTML elements
   - Stores tag name, text content, and child nodes
   - Recursive printing of HTML structure

## Planned Features
   - Attribute parsing to capture tag details (e.g., `class`, `id`)
   - Support for self-closing tags such as '<img/>, <br/>, <hr/> etc.
   - Graceful handling of malformed HTML
   - Modern memory management using `unique_ptr`
