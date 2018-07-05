#include "governancelist.h"
#include "ui_governancelist.h"
#include "../governance.h"
#include "../governance-object.h"

#include <QTimer>
#include <QMessageBox>



GovernanceList::GovernanceList(const PlatformStyle *platformStyle, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::GovernanceList),
        clientModel(0),
        walletModel(0)
{
    ui->setupUi(this);

    int columnAddressWidth = 200;
    int columnProtocolWidth = 60;
    int columnStatusWidth = 80;
    int columnActiveWidth = 130;
    int columnLastSeenWidth = 130;

    ui->tableWidgetGobjects->setColumnWidth(0, columnAddressWidth);
    ui->tableWidgetGobjects->setColumnWidth(1, columnProtocolWidth);
    ui->tableWidgetGobjects->setColumnWidth(2, columnStatusWidth);
    ui->tableWidgetGobjects->setColumnWidth(3, columnActiveWidth);
    ui->tableWidgetGobjects->setColumnWidth(4, columnLastSeenWidth);


    contextMenu = new QMenu();
    connect(ui->tableWidgetGobjects, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMyNodeList()));
    timer->start(1000);

    fFilterUpdated = false;
    nTimeFilterUpdated = GetTime();
}

GovernanceList::~GovernanceList()
{
    delete ui;
}
void GovernanceList::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void GovernanceList::showContextMenu(const QPoint &point)
{
    QTableWidgetItem *item = ui->tableWidgetGobjects->itemAt(point);
    if(item) contextMenu->exec(QCursor::pos());
}

void GovernanceList::updateGobjects()
{
    TRY_LOCK(cs_gobjlist, fLockAcquired);
    if(!fLockAcquired) {
        return;
    }

    static int64_t nTimeListUpdated = GetTime();

    // to prevent high cpu usage update only once in MASTERNODELIST_UPDATE_SECONDS seconds
    // or MASTERNODELIST_FILTER_COOLDOWN_SECONDS seconds after filter was last changed
    int64_t nSecondsToWait = fFilterUpdated
                            ? nTimeFilterUpdated - GetTime() + GOBJECT_UPDATE_SECONDS
                            : nTimeListUpdated - GetTime() + GOBJECT_UPDATE_SECONDS;

    if(fFilterUpdated) ui->countGobjectLabel->setText(QString::fromStdString(strprintf("Please wait... %d", nSecondsToWait)));
    if(nSecondsToWait > 0) return;

    nTimeListUpdated = GetTime();
    fFilterUpdated = false;

    QString strToFilter;
    ui->countGobjectLabel->setText("Updating...");
    ui->tableWidgetGobjects->setSortingEnabled(false);
    ui->tableWidgetGobjects->clearContents();
    ui->tableWidgetGobjects->setRowCount(0);
    //std::map<uint256, CGovernanceObject> mapGobject = governance.GetFullGobjectMap();

    //for (const auto& gobj : mapGobject)
    //{
        // populate list
        // Address, Protocol, Status, Active Seconds, Last Seen, Pub Key
        //QTableWidgetItem *nameItem = new QTableWidgetItem(QString::gobj("1"));
        //QTableWidgetItem *urlItem = new QTableWidgetItem(QString::gobj("1"));
        //QTableWidgetItem *amounItem = new QTableWidgetItem(QString::gobj("1"));
        //QTableWidgetItem *paymentAddrsItem = new QTableWidgetItem(QString::gobj("1"));
        //QTableWidgetItem *fundingStatus = new QTableWidgetItem(QString::gobj("1"));

    //   if (strCurrentFilter != "")
    //    {
    //        strToFilter =   nameItem->text() + " " +
    //                        urlItem->text() + " " +
    //                        amounItem->text() + " " +
    //                        paymentAddrsItem->text() + " " +
    //                        fundingStatus->text() + " " +
    //        if (!strToFilter.contains(strCurrentFilter)) continue;
    //    }

    //    ui->tableWidgetGobjects->insertRow(0);
    //    ui->tableWidgetGobjects->setItem(0, 0, nameItem);
    //    ui->tableWidgetGobjects->setItem(0, 1, urlItem);
    //    ui->tableWidgetGobjects->setItem(0, 2, amounItem);
    //    ui->tableWidgetGobjects->setItem(0, 3, paymentAddrsItem);
    //    ui->tableWidgetGobjects->setItem(0, 4, fundingStatus);
    //}

    ui->countGobjectLabel->setText(QString::number(ui->tableWidgetGobjects->rowCount()));
    ui->tableWidgetGobjects->setSortingEnabled(true);
}