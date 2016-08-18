//Author: Rémi Pelletier
//File:   EzSockets.h
//Desc.:  EzSockets dependencies header.

//TODO: Add headers and conditions for the Linux and OSX wrappers.

#pragma once
#include "EzSocket.h"

#ifdef _WIN32
#include "EzSocket_Win.h"
#endif
