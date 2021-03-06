#pragma once

#include <cule/config.hpp>
#include <cule/atari/controller.hpp>
#include <cule/atari/state.hpp>

namespace cule
{
namespace atari
{
namespace games
{
namespace krull
{

template<typename State>
CULE_ANNOTATION
 void reset(State& s)
{
    s.m_reward   = 0;
    s.m_score    = 0;
    s.tiaFlags.clear(FLAG_ALE_TERMINAL);
    s.m_lives 	 = 3;
}

template<typename State>
CULE_ANNOTATION
 void step(State& s)
{
    using cule::atari::games::getDecimalScore;
    using cule::atari::ram::read;

    // update the reward
    int score = getDecimalScore(s, 0x9E, 0x9D, 0x9C);
    int reward = score - s.m_score;
    s.m_reward = reward;
    s.m_score = score;

    // update terminal status
    int lives = ram::read(s, 0x9F);
    int byte1 = ram::read(s, 0xA2);
    int byte2 = ram::read(s, 0x80);
    s.tiaFlags.template change<FLAG_ALE_TERMINAL>((lives == 0) && (byte1 == 0x03) && (byte2 == 0x80));
    s.m_lives = (lives & 0x7) + 1;
}

CULE_ANNOTATION
bool isMinimal(const Action &a)
{
    switch (a)
    {
    case ACTION_NOOP:
    case ACTION_FIRE:
    case ACTION_UP:
    case ACTION_RIGHT:
    case ACTION_LEFT:
    case ACTION_DOWN:
    case ACTION_UPRIGHT:
    case ACTION_UPLEFT:
    case ACTION_DOWNRIGHT:
    case ACTION_DOWNLEFT:
    case ACTION_UPFIRE:
    case ACTION_RIGHTFIRE:
    case ACTION_LEFTFIRE:
    case ACTION_DOWNFIRE:
    case ACTION_UPRIGHTFIRE:
    case ACTION_UPLEFTFIRE:
    case ACTION_DOWNRIGHTFIRE:
    case ACTION_DOWNLEFTFIRE:
        return true;
    default:
        return false;
    }
}

template<typename State>
CULE_ANNOTATION
int32_t lives(State& s)
{
    return cule::atari::ram::read(s.ram, 0x9F) + 1;
}

template<typename State>
CULE_ANNOTATION
 void setTerminal(State& s)
{
    using cule::atari::ram::read;

    // update terminal status
    int lives = ram::read(s.ram, 0x9F);
    int byte1 = ram::read(s.ram, 0xA2);
    int byte2 = ram::read(s.ram, 0x80);
    s.tiaFlags.template change<FLAG_ALE_TERMINAL>((lives == 0) && (byte1 == 0x03) && (byte2 == 0x80));
}

template<typename State>
CULE_ANNOTATION
int32_t score(State& s)
{
    return cule::atari::games::getDecimalScore(s, 0x9E, 0x9D, 0x9C);
}

template<typename State>
CULE_ANNOTATION
int32_t reward(State& s)
{
    return score(s) - s.score;
}

} // end namespace krull
} // end namespace games
} // end namespace atari
} // end namespace cule

