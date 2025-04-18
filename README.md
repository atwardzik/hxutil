# hxutil
hxutil is a simple IDE best suited for embedded and low level programming. It includes integration with user specified C and Assembly compilers.

Features:
- syntax coloring
- code formatter
- disassembly/hexdump view

### Cloning
Please note that the project is primarily maintained on [GitLab](https://gitlab.com/atwardzik/hxdump) platform. Therefore, it is crucial after cloning from [GitHub](https://github.com/atwardzik/hxutil/) to run those commands:
```console
$ cd include/asmstatic
$ git submodule sync
$ git submodule update --init --recursive
```
If you clone from GitLab there is no need for running those commands, as the project should be cloned properly.

### License
This tool is licensed under GNU GPL v3.0.

Copyright (C) 2025  Artur Twardzik
