/*****************************************************************************
 * Copyright (C) 2005 Eliot
 * Authors: Olivier Teuliere  <ipkiss@via.ecp.fr>
 *
 * $Id: game_factory.cpp,v 1.3 2005/03/28 22:07:23 ipkiss Exp $
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

#include <getopt.h>

#include "config.h"
#include "dic.h"
#include "game_factory.h"


GameFactory *GameFactory::m_factory = NULL;


GameFactory::GameFactory(): m_dic(NULL), m_human(0), m_ai(0), m_joker(false)
{
}


GameFactory::~GameFactory()
{
    if (m_dic)
        Dic_destroy(m_dic);
}


GameFactory *GameFactory::Instance()
{
    if (m_factory == NULL)
        m_factory = new GameFactory;
    return m_factory;
}


void GameFactory::Destroy()
{
    if (m_factory)
        delete m_factory;
    m_factory = NULL;
}


Training *GameFactory::createTraining(const Dictionary &iDic)
{
    Training *game = new Training(iDic);
    return game;
}


FreeGame *GameFactory::createFreeGame(const Dictionary &iDic)
{
    FreeGame *game = new FreeGame(iDic);
    return game;
}


Duplicate *GameFactory::createDuplicate(const Dictionary &iDic)
{
    Duplicate *game = new Duplicate(iDic);
    return game;
}


Game *GameFactory::createFromCmdLine(int argc, char **argv)
{
    // 1) Parse command-line and store everything in member variables
    static struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"dictionary", required_argument, NULL, 'd'},
        {"dict", required_argument, NULL, 'd'},
        {"mode", required_argument, NULL, 'm'},
        {"human", no_argument, NULL, 300},
        {"ai", no_argument, NULL, 400},
        {"joker", no_argument, NULL, 500},
        {0, 0, 0, 0}
    };
    static char short_options[] = "hvd:m:";

    int option_index = 1;
    int res;
    while ((res = getopt_long(argc, argv, short_options,
                              long_options, &option_index)) != -1)
    {
        switch (res)
        {
        case 'h':
            // Help requested, display it and exit
            printUsage(argv[0]);
            return NULL;
        case 'v':
            // Version requested, display it and exit
            printVersion();
            return NULL;
        case 'd':
            m_dicStr = optarg;
            break;
        case 'm':
            m_modeStr = optarg;
            break;
        case 300:
            m_human++;
            break;
        case 400:
            m_ai++;
            break;
        case 500:
            m_joker = true;
            break;
        }
    }

    // 2) Try to load the dictionary
    if (Dic_load(&m_dic, m_dicStr.c_str()))
    {
        cerr << "Could not load dictionary '" << m_dicStr << "'\n";
        return NULL;
    }

    // 3) Try to create a game object
    Game *game = NULL;
    if (m_modeStr == "training" || m_modeStr == "t")
    {
        game = createTraining(m_dic);
    }
    else if (m_modeStr == "freegame" || m_modeStr == "f")
    {
        game = createFreeGame(m_dic);
    }
    else if (m_modeStr == "duplicate" || m_modeStr == "d")
    {
        game = createDuplicate(m_dic);
    }
    else
    {
        cerr << "Invalid game mode '" << m_modeStr << "'\n";
        return NULL;
    }

    // 4) Add the players
    for (int i = 0; i < m_human; i++)
        game->addHumanPlayer();
    for (int i = 0; i < m_ai; i++)
        game->addAIPlayer();

    // 5) Set the variant
    if (m_joker)
        game->setVariant(Game::kJOKER);

    return game;
}


void GameFactory::releaseGame(Game &iGame)
{
    delete &iGame;
}


void GameFactory::printUsage(const string &iBinaryName) const
{
    cout << "Usage: " << iBinaryName << " [options]\n"
         << "\n"
         << "  -h, --help               Print this help and exit\n"
         << "  -v, --version            Print version information and exit\n"
         << "  -m, --mode {duplicate,d,freegame,f,training,t}\n"
         << "                           Choose game mode\n"
         << "  -d, --dict <string>      Choose a dictionary\n"
         << "      --human              Add a human player\n"
         << "      --ai                 Add a AI (Artificial Intelligence) player\n"
         << "      --joker              Play with the \"Joker game\" variant\n";
}


void GameFactory::printVersion() const
{
    cout << PACKAGE_STRING << "\n"
         << "This program comes with NO WARRANTY, to the extent permitted by "
         << "law.\nYou may redistribute it under the terms of the GNU General "
         << "Public License;\nsee the file named COPYING for details.\n";
}
