    //
// Created by roman sztergbaum on 09/10/2017.
//

#ifndef GAME_TEMPLATE_ENUMSCENES_HPP
#define GAME_TEMPLATE_ENUMSCENES_HPP

namespace rtype
{
    enum class Scene : char
    {
        //! Scenes Name Here
        NoScene = -1,
        Intro = 0,
        Login = 1,
        Create = 2,
        Profil = 3,
        Loading = 4,
        Queue = 5,
        Lobby = 6,
        Demo = 7,
        Game = 8,
    };
}

#endif //GAME_TEMPLATE_ENUMSCENES_HPP
