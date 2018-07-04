#ifndef GOVERNANCELIST_H
#define GOVERNANCELIST_H

#include "primitives/transaction.h"
#include "platformstyle.h"
#include "sync.h"
#include "util.h"

#include <QMenu>
#include <QTimer>
#include <QWidget>


namespace Ui {
    class GovernanceList;
}

class ClientModel;
class WalletModel;

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE;

/** Governance Manager page widget */
class GovernanceList : public QWidget
{
    Q_OBJECT

};
#endif // GOVERNANCELIST_H
