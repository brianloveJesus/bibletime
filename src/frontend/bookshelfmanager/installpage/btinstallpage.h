/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BTINSTALLPAGE_H
#define BTINSTALLPAGE_H

#include "frontend/bookshelfmanager/btconfigdialog.h"


class BtSourceWidget;
class QComboBox;
class QPushButton;
class QToolButton;

/**
* The Install page includes module path chooser, source/module handler and install button.
*/
class BtInstallPage : public BtConfigPage {
        Q_OBJECT
    public:
        BtInstallPage(QWidget *parent = 0);

        /** Reimplemented from BtConfigPage. */
        virtual const QIcon &icon() const;

        /** Reimplemented from BtConfigPage. */
        virtual QString label() const;

        /** Reimplemented from BtConfigPage. */
        virtual QString header() const;

        void setInstallEnabled(bool b);

        QString selectedInstallPath();

    public slots:
        void slotSwordSetupChanged();

    private:
        void initView();
        void initConnections();
        void initPathCombo();

    private slots:
        void slotPathChanged(const QString& pathText);
        void slotEditPaths();

    private:

        QComboBox* m_pathCombo;
        QToolButton* m_configurePathButton;
        BtSourceWidget* m_sourceWidget;
        QPushButton* m_installButton;
};

#endif