<!--
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
-->

# Ctache

Ctache (pronounced "stash" as in "stache" as in "moustache" you see) is an
implementation of [mustache](https://mustache.github.io/) templates in C
(though I have not yet checked whether it fully implements the spec). It is
available as the command-line tool `ctache` and also in library form as
`libctache`.

## Building & Installing

Ctache requires a C99-compatible C compiler -- it should work with both GCC and
clang in any reasonably modern version thereof. I've tested it with both
GCC-4.8 and clang-3.6 and they both work.

### Dependencies

Ctache has only one dependency other than a working C99-compatible compiler.
It requires the libyaml library. At the time of this writing the version of the
library is 0.1.6 and it is available [here](http://pyyaml.org/wiki/LibYAML). It
should be available in the package manager for your operating system. I got it
from [pkgsrc](https://www.pkgsrc.org/) on my OS X machine, apt-get on my Ubuntu
machine and from the ports tree on my FreeBSD machine.

### Compiling

If you downloaded an archive of Ctache that does not include an executable
`configure` script, you'll need the Autotools to generate it. To do that, run

```sh
autoreconf -iv
```

If a `configure` script already exists, you can compile and install Ctache as
follows (note that you may need to be root to run the install step):

```sh
./configure
make
make install
```

## Usage

This section explains how to use Ctache.

### Command-Line Tool

Note: By default, `ctache` reads input from `stdin` and writes output to
`stdout`.

To render an HTML template:

```sh
ctache -y data.yml -i template.html -o output.html
```

### Library Use

For an explanation of the Ctache library, libctache, see the man pages
`ctache_render(3)` and `ctache_data(3)`. These man pages are installed with
Ctache.

## License

Ctache is licensed under the [MPLv2](https://www.mozilla.org/en-US/MPL/2.0/).
If you don't know how the MPLv2 works, there is an
[FAQ](https://www.mozilla.org/en-US/MPL/2.0/FAQ/). Note that the version of
the MPLv2 used by the Ctache project *is* compatible with the GPL. If you want
to use the code from Ctache in a GPL'd project, that's perfectly fine. A copy
of the MPLv2 should be included with this project in a file called
`LICENSE.md`.
