/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2016 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "btsourcesthread.h"

#include <QString>
#include <memory>
#include <type_traits>
#include "btinstallbackend.h"
#include "backend/btinstallmgr.h"
#include "util/btassert.h"


void BtSourcesThread::run() {
    emit percentComplete(0);
    emit showMessage(tr("Getting Library List"));
    if (BtInstallMgr().refreshRemoteSourceConfiguration())
        qWarning("InstallMgr: getting remote list returned an error.");
    emit percentComplete(10);

    if (shouldStop()) {
        emit showMessage(tr("Updating stopped"));
        return;
    }

    QStringList const sourceNames = BtInstallBackend::sourceNameList();
    auto const sourceCount = sourceNames.count();
    BT_ASSERT(sourceCount >= 0);
    std::unique_ptr<int[]> failedSources{new int[sourceCount]};
    std::size_t numFailedSources = 0u;
    BtInstallMgr iMgr;
    for (int i = 0; i < sourceCount; ++i) {
        if (shouldStop()) {
            emit showMessage(tr("Updating stopped"));
            return;
        }
        QString const & sourceName = sourceNames[i];
        emit showMessage(tr("Updating remote library \"%1\"").arg(sourceName));
        {
            sword::InstallSource source = BtInstallBackend::source(sourceName);
            if (iMgr.refreshRemoteSource(&source)) {
                failedSources[numFailedSources] = i;
                ++numFailedSources;
            }
        }
        emit percentComplete(
                    static_cast<int>(10 + 90 * ((i + 1.0) / sourceCount)));
    }
    emit percentComplete(100);
    if (numFailedSources <= 0) {
        emit showMessage(tr("Remote libraries have been updated."));
        m_finishedSuccessfully.store(true, std::memory_order_release);
    } else {
        QString msg = tr("The following remote libraries failed to update: ");
        for (std::size_t i = 0;;) {
            msg += sourceNames[failedSources[i]];
            if (++i >= numFailedSources)
                break;
            msg += ", ";
        };
        emit showMessage(std::move(msg));
        m_finishedSuccessfully.store(true, std::memory_order_release);
    }
}
