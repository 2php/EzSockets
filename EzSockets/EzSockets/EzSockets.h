//Author: Rémi Pelletier
//File:   EzSockets.h
//Desc.:  EzSockets dependencies header.

//TODO: Add POSIX EzSocket implementation header.

#pragma once
#include "EzSocket.h"

#ifdef _WIN32
#include "EzSocket_Win.h"
#endif