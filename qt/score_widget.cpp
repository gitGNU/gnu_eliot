/*****************************************************************************
 * Eliot
 * Copyright (C) 2008-2012 Olivier Teulière
 * Authors: Olivier Teulière <ipkiss @@ gmail.com>
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *****************************************************************************/

#include <QTreeView>
#include <QStandardItemModel>

#include "score_widget.h"
#include "qtcommon.h"
#include "public_game.h"
#include "player.h"

using namespace std;


INIT_LOGGER(qt, ScoreWidget);


ScoreWidget::ScoreWidget(QWidget *parent, const PublicGame *iGame)
    : QTreeView(parent), m_game(iGame)
{
    // Create the tree view
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setRootIsDecorated(false);
    setSortingEnabled(true);
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setSizePolicy(policy);

    // Associate the model to the view
    m_model = new QStandardItemModel(this);
    setModel(m_model);
    m_model->setColumnCount(2);
    m_model->setHeaderData(0, Qt::Horizontal, _q("Player"), Qt::DisplayRole);
    m_model->setHeaderData(1, Qt::Horizontal, _q("Score"), Qt::DisplayRole);
    updateModel();
}


void ScoreWidget::setGame(const PublicGame *iGame)
{
    m_game = iGame;
    updateModel();
}


void ScoreWidget::refresh()
{
    updateModel();
}


void ScoreWidget::updateModel()
{
    m_model->removeRows(0, m_model->rowCount());
    resizeColumnToContents(0);
    //resizeColumnToContents(1);

    if (m_game == NULL)
        return;

    for (unsigned int i = 0; i < m_game->getNbPlayers(); ++i)
    {
        const Player &p = m_game->getPlayer(i);
        int rowNum = m_model->rowCount();
        m_model->insertRow(rowNum);
        m_model->setData(m_model->index(rowNum, 0), qfw(p.getName()));
        m_model->setData(m_model->index(rowNum, 1), p.getTotalScore());
    }
    resizeColumnToContents(0);
    //resizeColumnToContents(1);
}


QSize ScoreWidget::sizeHint() const
{
    return QSize(160, 100);
}

