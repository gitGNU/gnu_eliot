/*****************************************************************************
 * Copyright (C) 2004-2005 Eliot
 * Authors: Olivier Teuliere  <ipkiss@via.ecp.fr>
 *
 * $Id: player.h,v 1.6 2005/03/27 17:30:48 ipkiss Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>

class Playedrack;
class Round;
class Board;
typedef struct _Dictionary * Dictionary;


/**
 * This class is the parent classes for all the players involved in a game.
 * It defines the common metohds to update the rack, score, etc...
 */
class Player
{
public:
    Player();
    virtual ~Player();

    // Pseudo RTTI
    virtual bool isHuman() const = 0;

    /**************************
     * General getters
     **************************/
    // Get the (possibly incomplete) rack of the player
    const PlayedRack & getCurrentRack() const;
    // Get the previous rack
    const PlayedRack & getLastRack() const;
    // Get the previous round (corresponding to the previous rack...)
    const Round & getLastRound() const;

    void setCurrentRack(const PlayedRack &iPld);

    /**************************
     * Acessors for the score of the player
     **************************/
    // Add (or remove, if iPoints is negative) points to the score
    // of the player
    void addPoints(int iPoints) { m_score += iPoints; }
    int  getPoints() const      { return m_score; }

    // Update the player "history", with the given round.
    // A new rack is created with the remaining letters
    void endTurn(const Round &iRound, int iTurn);

private:
    // Score of the player
    int m_score;

    // History of the racks and rounds for the player
    vector<PlayedRack *> m_playedRacks;
    vector<Round *> m_rounds;
    vector<int> m_turns;
};


/**
 * Human player.
 */
class HumanPlayer: public Player
{
public:
    HumanPlayer() {}
    virtual ~HumanPlayer() {}

    // Pseudo RTTI
    virtual bool isHuman() const { return true; }
};

#endif
