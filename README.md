# EzSockets  
**Author :** Rémi Pelletier  
**Description :** Simple cross-platform sockets wrapper (Work in progress).  

## Goal  
Develop an easy-to-use cross-platform sockets wrapper.    

## Motivations  
- Developing a better understanding of networking and sockets.
- Improving my knowledge of Berkeley sockets.
- Improving my knowledge of the Windows sockets API (WSA / Winsock).
- Improving my knowledge of Linux and OSX implementations of POSIX sockets.

  
## Notice  
This project is not currently finished, it may contain errors, bugs and inaccuracies. The source code was published at this state in order to showcase the progress I have made on this project. Every file on the repository is therefore subject to change.

The project currently only supports Winsock sockets, support for the Linux and OSX implementations of POSIX sockets will be coming shortly.

The current implementation uses an abstract class and factory pattern, it might be replaced by a Pointer to implementation (PIMPL) pattern.
  
## License   
Licensed under the Apache License, Version 2.0, ([LICENSE](LICENSE) or http://www.apache.org/licenses/LICENSE-2.0).
