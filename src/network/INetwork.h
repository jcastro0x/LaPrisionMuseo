// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <vector>

#define DECLARE_OBSERVER(Name) void(*Name)()
#define DECLARE_OBSERVER_OneParam(Name,Param1) void(*Name)(Param1)
#define DECLARE_OBSERVER_TwoParam(Name,Param1,Param2) void(*Name)(Param1,Param2)
#define DECLARE_OBSERVER_ThreeParam(Name,Param1,Param2,Param3) void(*Name)(Param1,Param2,Param3)

class INetwork
{
public:
    virtual ~INetwork() = default;

    virtual void init() = 0;

    virtual void changeRoom(class RoomSceneNode* room) = 0;
    virtual void sendMessage(class Player* player, const char* message) = 0;
    virtual void sendMessage(const char* message) = 0;

public:
    struct Observers
    {
        //
        // Connection
        // ~=======================================================================================
        DECLARE_OBSERVER(Connected);
        DECLARE_OBSERVER(Disconnected);
        DECLARE_OBSERVER_OneParam(ConnectionError, const char* /*reason*/);
        DECLARE_OBSERVER_OneParam(ConnectionKicked, const char* /*reason*/);
        DECLARE_OBSERVER_OneParam(LoginStatus, bool /*success*/);
        DECLARE_OBSERVER_OneParam(RoomChanged, class RoomSceneNode* /*success*/);

        //
        // Players
        // ~=======================================================================================
        DECLARE_OBSERVER_OneParam(PlayerEnterRoom, class Player* /*player*/);
        DECLARE_OBSERVER_OneParam(PlayerLeaveRoom, class Player* /*player*/);
        DECLARE_OBSERVER_TwoParam(PlayerEnterCamera, class Player* /*player*/, const char* /*camera*/);
        DECLARE_OBSERVER_TwoParam(PlayerLeaveCamera, class Player* /*player*/, const char* /*camera*/);
        DECLARE_OBSERVER_OneParam(PlayersRoomList, std::vector<class Player*> /*players*/);
        DECLARE_OBSERVER_ThreeParam(PlayerPosition, class Player* /*player*/, unsigned /*posX*/, unsigned /*posY*/);

        //
        // Chat
        // ~=======================================================================================
        DECLARE_OBSERVER_OneParam(GlobalMessage, const char* /*message*/);
        DECLARE_OBSERVER_TwoParam(PlayerMessage, class Player* /*player*/, const char* /*message*/);
        DECLARE_OBSERVER_TwoParam(PrivateMessage, class Player* /*player*/, const char* /*message*/);

        //
        // Room
        // ~=======================================================================================
        DECLARE_OBSERVER_OneParam(RoomMessage, const char* /*message*/);
        DECLARE_OBSERVER_OneParam(SpawnItem, class RoomItem* /*item*/);
        DECLARE_OBSERVER_OneParam(DestroyItem, class RoomItem* /*item*/);

    } observers_;
};

#undef DECLARE_OBSERVER
#undef DECLARE_OBSERVER_OneParam
#undef DECLARE_OBSERVER_TwoParam
#undef DECLARE_OBSERVER_ThreeParam
