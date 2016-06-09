<!--
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
-->

# Lexing & Parsing

This document explains how the Ctache lexical analysis and parsing work, at a
relatively high level.

## Lexical Analysis

The symbols of Ctache's dialect of mustache templates are defined as follows
(note that the precedence is top-down and that `*` is a wildcard):

```
SECTION_TAG_START = {{#
CLOSE_TAG_START   = {{/
VALUE_VALUE_START = {{
TAG_END           = }}
STRING            = *
```

## Parsing

Ctache uses an [LL(1)](https://en.wikipedia.org/wiki/LL_parser) table-based
parser.

### Grammar

The grammar of Ctache's mustache dialect is defined in rough
[EBNF](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form) below:

```
template  = text, template | tag, template
text      = string | ""
tag       = tag start, string, tag end
tag start = section tag start | close tag start | value tag start
```

The rules that follow from the above grammar as as follows:

1. `template  -> text, template`
2. `template  -> tag, template`
3. `text      -> string`
4. `text      -> ""`
5. `tag       -> tag start, string, tag end`
6. `tag start -> section tag start`
7. `tag start -> close tag start`
8. `tag start -> value tag start`

### Parsing Table

|           | {{# | {{/ | {{ | }} | string | $ |
|-----------|-----|-----|----|----|--------|---|
| template  |  2  |  -  |  2 |  - |    1   | - |
| text      |  4  |  4  |  4 |  4 |    3   | 4 |
| tag       |  5  |  5  |  5 |  - |    -   | - |
| tag start |  6  |  7  |  8 |  - |    -   | - |

Note that when the parsing table is translated into actual C code, the rule
indices will be zero-indexed, as opposed to beginning the indices at one. This
is a necessity of C's array indexing.

