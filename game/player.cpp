/*****************************************************************************
 * Copyright (C) 2004-2005 Eliot
 * Authors: Olivier Teuliere  <ipkiss@via.ecp.fr>
 *
 * $Id: player.cpp,v 1.4 2005/02/17 20:01:59 ipkiss Exp $
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

#include "tile.h"
#include "rack.h"
#include "pldrack.h"
#include "round.h"
#include "results.h"
#include "board.h"
#include "player.h"

#include "debug.h"

Player::Player():
    m_score(0)
{
    // Start with an empty rack
    m_playedRacks.push_back(new PlayedRack());
}


Player::~Player()
{
    for (unsigned int i = 0; i < m_playedRacks.size(); i++)
        delete m_playedRacks[i];
    for (unsigned int i = 0; i < m_rounds.size(); i++)
        delete m_rounds[i];
}


const PlayedRack & Player::getCurrentRack() const
{
    return *m_playedRacks.back();
}


void Player::setCurrentRack(const PlayedRack &iPld)
{
    *m_playedRacks.back() = iPld;
}


const PlayedRack & Player::getLastRack() const
{
    return *m_playedRacks[m_playedRacks.size() - 2];
}


const Round & Player::getLastRound() const
{
    return *m_rounds.back();
}


/*
 * This function increments the number of racks, and fills the new rack
 * with the unplayed tiles from the previous one.
 * 03 sept 2000 : We have to sort the tiles according to the new rules
 */
void Player::endTurn(const Round &iRound, int iTurn)
{
    m_turns.push_back(iTurn);
    m_rounds.push_back(new Round(iRound));

    Rack rack;
    m_playedRacks.back()->getRack(rack);

    /* We remove the played tiles from the rack */
    for (int i = 0; i < iRound.getWordLen(); i++)
    {
        if (iRound.isPlayedFromRack(i))
        {
            if (iRound.isJoker(i))
                rack.remove(Tile::Joker());
            else
                rack.remove(iRound.getTile(i));
        }
    }

    m_playedRacks.push_back(new PlayedRack());
    /* Now m_playedRacks.back() is the newly created PlayedRack object */
    m_playedRacks.back()->setOld(rack);
}
