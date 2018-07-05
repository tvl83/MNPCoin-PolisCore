#include "governancelist.h"
#include "ui_governancelist.h"
#include "../governance.h"
#include "../governance-object.h"
#include "clientmodel.h"

#include <QTimer>
#include <QMessageBox>



GovernanceList::GovernanceList(const PlatformStyle *platformStyle, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::GovernanceList),
        clientModel(0),
        walletModel(0)
{
    ui->setupUi(this);

    int columnName = 200;
    int columnUrl = 200;
    int columnAmount = 120;
    int columnPayment = 250;
    int columnvoteYes = 80;
    int columnvoteNo = 80;
    int columnAbsoluteYes = 150;
    int columnFund = 50;

    ui->tableWidgetGobjects->setColumnWidth(0, columnName);
    ui->tableWidgetGobjects->setColumnWidth(1, columnUrl);
    ui->tableWidgetGobjects->setColumnWidth(2, columnAmount);
    ui->tableWidgetGobjects->setColumnWidth(3, columnPayment);
    ui->tableWidgetGobjects->setColumnWidth(4, columnvoteYes);
    ui->tableWidgetGobjects->setColumnWidth(5, columnvoteNo);
    ui->tableWidgetGobjects->setColumnWidth(6, columnAbsoluteYes);
    ui->tableWidgetGobjects->setColumnWidth(7, columnFund);


    contextMenu = new QMenu();
    connect(ui->tableWidgetGobjects, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGobjects()));
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
    int nStartTime = 0; //list
    std::vector<const CGovernanceObject*> objs = governance.GetAllNewerThan(nStartTime);

    for (const auto& pGovObj : objs)
    {

        // Define "Funding" for Vote count
        vote_signal_enum_t VoteCountType = vote_signal_enum_t(1);

        // Convert "Funding" Boolean to std::string
        int gobject = pGovObj->GetObjectType();
        std::string vFunding;
        if (pGovObj->IsSetCachedFunding()) {
            vFunding = "Yes";
        } else {vFunding = "No";}

        QString name = QString::fromStdString(pGovObj->GetDataAsPlainString());
        QString url = QString::fromStdString(pGovObj->GetDataAsPlainString());
        QString amount = QString::fromStdString(pGovObj->GetDataAsPlainString());
        QString address = QString::fromStdString(pGovObj->GetDataAsPlainString());

        if (gobject == 1) {
        // populate list
        // Address, Protocol, Status, Active Seconds, Last Seen, Pub Key
        QTableWidgetItem *nameItem = new QTableWidgetItem(name);
        QTableWidgetItem *urlItem = new QTableWidgetItem(url);
        QTableWidgetItem *amounItem = new QTableWidgetItem(amount);
        QTableWidgetItem *paymentAddrsItem = new QTableWidgetItem(address);
        QTableWidgetItem *voteYes = new QTableWidgetItem(QString::number(pGovObj->GetYesCount(VoteCountType)));
        QTableWidgetItem *voteNo = new QTableWidgetItem(QString::number(pGovObj->GetNoCount(VoteCountType)));
        QTableWidgetItem *AbsoluteYes = new QTableWidgetItem(QString::number(pGovObj->GetAbsoluteYesCount(VoteCountType)));
        QTableWidgetItem *fundingStatus = new QTableWidgetItem(QString::fromStdString(vFunding));

       if (strCurrentFilter != "")
        {
            strToFilter =   nameItem->text() + " " +
                            amounItem->text() + " " +
                            paymentAddrsItem->text() + " " +
                            voteYes->text() + " " +
                            voteNo->text() + " " +
                            AbsoluteYes->text() + " " +
                            fundingStatus->text();
            if (!strToFilter.contains(strCurrentFilter)) continue;
        }

        ui->tableWidgetGobjects->insertRow(0);
        ui->tableWidgetGobjects->setItem(0, 0, nameItem);
        ui->tableWidgetGobjects->setItem(0, 1, urlItem);
        ui->tableWidgetGobjects->setItem(0, 2, amounItem);
        ui->tableWidgetGobjects->setItem(0, 3, paymentAddrsItem);
        ui->tableWidgetGobjects->setItem(0, 4, voteYes);
        ui->tableWidgetGobjects->setItem(0, 5, voteNo);
        ui->tableWidgetGobjects->setItem(0, 6, AbsoluteYes);
        ui->tableWidgetGobjects->setItem(0, 7, fundingStatus);
        }
    }

    ui->countGobjectLabel->setText(QString::number(ui->tableWidgetGobjects->rowCount()));
    ui->tableWidgetGobjects->setSortingEnabled(true);
}

void GovernanceList::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void GovernanceList::on_UpdateButton_clicked()
{
    updateGobjects();
}