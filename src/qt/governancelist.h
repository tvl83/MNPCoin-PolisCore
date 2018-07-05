#ifndef GOVERNANCELIST_H
#define GOVERNANCELIST_H

#include "primitives/transaction.h"
#include "platformstyle.h"
#include "sync.h"
#include "util.h"

#include <QMenu>
#include <QTimer>
#include <QWidget>

#define GOBJECT_UPDATE_SECONDS                 60

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

public:
    explicit GovernanceList(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~GovernanceList();
    void setWalletModel(WalletModel *walletModel);

private:
    QMenu *contextMenu;
    int64_t nTimeFilterUpdated;
    bool fFilterUpdated;

public Q_SLOTS:
    void updateGobjects();

private:
    QTimer *timer;
    Ui::GovernanceList *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;

    // Protects tableWidgetMasternodes
    CCriticalSection cs_gobjlist;

    QString strCurrentFilter;

private Q_SLOTS:
    void showContextMenu(const QPoint &);
    void on_filterLineEdit_textChanged(const QString &strFilterIn);
    void on_QRButton_clicked();
    void on_startButton_clicked();
    void on_startAllButton_clicked();
    void on_startMissingButton_clicked();
    void on_tableWidgetMyMasternodes_itemSelectionChanged();
    void on_UpdateButton_clicked();
};
#endif // GOVERNANCELIST_H
