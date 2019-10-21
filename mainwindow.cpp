#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    open();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Definite da me

//Per il caricamento del file data.txt
void MainWindow::open(){
    QFile file("data.txt"); //PRJ: DECISONE non faccio selezionare all'utente il file da caricare perché nel testo diceva il file data.txt

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Unable to open file"),
                              file.errorString());
        return;
    }

    // Leggo da file il testo
    QTextStream readData(&file);
    QString fileText = readData.readAll();

    //split per righe, ottengo una lista con tutte le info
    list = fileText.split("\n", QString::SkipEmptyParts); //con una riga per ogni cella

    //rimuovo <END> dalla lista:
    if(list.last()=="<END>"){
        list.removeLast();
    }

    QStringList regioni;
    for(int i=0; i<list.length(); i++){
        QString desired = list[i].mid(0, list[i].indexOf(" "));
        regioni.append(desired);
    }
    QStringList regioniSingole = regioni.toSet().toList(); //non mi tiene però l'ordine
    std::sort(regioniSingole.begin(),regioniSingole.end()); //provo a riordinare

    //Inserimento regioni singole nella combobox
    ui->comboBox->addItems(regioniSingole);

    //imposto la tabella con il valore iniziale della combobox
    aggiornaTabellaGrafici(ui->comboBox->currentText());

    file.close();
}

//quando la combobox viene utilizzata
void MainWindow::on_comboBox_activated(const QString &arg1)
{
    //estrapola la regione selezionata e aggiorna i grafici e la tabella
    QString regioneSelezionata = arg1;
    aggiornaTabellaGrafici(regioneSelezionata);
}

//aggiorno la tabella e i grafici
void MainWindow::aggiornaTabellaGrafici(const QString &regioneSelezionata){
    //scelgo di inserire in liste età, donne,uomini invece che singoli elementi,
    //perché nel caso in cui li debba riutilizzare è più comodo averli tutti
    QStringList etaRegioneSelezionata;
    QStringList donneRegioneSelezionata;
    QStringList uominiRegioneSelezionata;
    int righeTotali=0;
    //conta righe da inserire in tabella
    for(int i=0; i<list.length(); i++){
        QStringList regioneEtaDonneUomini = list[i].split(" ", QString::SkipEmptyParts);
        if(regioneEtaDonneUomini[0]==regioneSelezionata){
            righeTotali++;
        }
    }
    ui->tableWidget->setRowCount(righeTotali+1); //+1 per la riga del totale

    //crea grafici e spicchi
    QChart *chartUomini = new QChart;
    QChart *chartDonne = new QChart;

    chartDonne->setTheme(QChart::ChartThemeBlueCerulean);
    chartUomini->setTheme(QChart::ChartThemeDark);

    QPieSeries *serieUomini = new QPieSeries;
    QPieSeries *serieDonne = new QPieSeries;

    int totaleUomini=0;
    int totaleDonne=0;
    int riga=0;
    for(int i=0; i<list.length(); i++){
        QStringList regioneEtaDonneUomini = list[i].split(" ", QString::SkipEmptyParts);
        if(regioneEtaDonneUomini[0]==regioneSelezionata){
            etaRegioneSelezionata.append(regioneEtaDonneUomini[1]);
            donneRegioneSelezionata.append(regioneEtaDonneUomini[2]);
            uominiRegioneSelezionata.append(regioneEtaDonneUomini[3]);
            //creo l'item da inserire nella table e lo inserisco nella table
            QTableWidgetItem* dataItemEtaRegioneSelezionata = new  QTableWidgetItem(etaRegioneSelezionata.last());
            QTableWidgetItem* dataItemDonneRegioneSelezionata = new  QTableWidgetItem(donneRegioneSelezionata.last());
            QTableWidgetItem* dataItemUominiRegioneSelezionata = new  QTableWidgetItem(uominiRegioneSelezionata.last());
            dataItemEtaRegioneSelezionata->setFlags(dataItemEtaRegioneSelezionata->flags() ^ Qt::ItemIsEditable);
            dataItemDonneRegioneSelezionata->setFlags(dataItemDonneRegioneSelezionata->flags() ^ Qt::ItemIsEditable);
            dataItemUominiRegioneSelezionata->setFlags(dataItemUominiRegioneSelezionata->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(riga,0,dataItemEtaRegioneSelezionata);
            ui->tableWidget->setItem(riga,2,dataItemDonneRegioneSelezionata);
            ui->tableWidget->setItem(riga,1,dataItemUominiRegioneSelezionata);
            //aggiorno i totali di donne e uomini per la regione attuale
            totaleDonne = totaleDonne + donneRegioneSelezionata.last().toInt();
            totaleUomini = totaleUomini + uominiRegioneSelezionata.last().toInt();
            riga++;
            //aggiungo alle serie che verranno poi inserite nei grafici
            serieUomini->append(etaRegioneSelezionata.last(), uominiRegioneSelezionata.last().toInt());
            serieDonne->append(etaRegioneSelezionata.last(), donneRegioneSelezionata.last().toInt());
        }
    }
    //aggiungo la riga del totale:
    QTableWidgetItem* totaleItemRegioneSelezionata = new  QTableWidgetItem("Totale:");
    QTableWidgetItem* totaleItemDonneRegioneSelezionata = new  QTableWidgetItem(QString::number(totaleDonne));
    QTableWidgetItem* totaleItemUominiRegioneSelezionata = new  QTableWidgetItem(QString::number(totaleUomini));
    totaleItemRegioneSelezionata->setFlags(totaleItemRegioneSelezionata->flags() ^ Qt::ItemIsEditable);
    totaleItemDonneRegioneSelezionata->setFlags(totaleItemDonneRegioneSelezionata->flags() ^ Qt::ItemIsEditable);
    totaleItemUominiRegioneSelezionata->setFlags(totaleItemUominiRegioneSelezionata->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(riga,0,totaleItemRegioneSelezionata);
    ui->tableWidget->setItem(riga,2,totaleItemDonneRegioneSelezionata);
    ui->tableWidget->setItem(riga,1,totaleItemUominiRegioneSelezionata);

    //faccio riempire tutto lo spazio della tabella che avanza all'ultima colonna
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    //inserisco le serie nei grafici
    chartUomini->addSeries(serieUomini);
    chartUomini->setTitle(tr("Uomini"));
    chartUomini->setAnimationOptions(QChart::AllAnimations);

    chartDonne->addSeries(serieDonne);
    chartDonne->setTitle(tr("Donne"));
    chartDonne->setAnimationOptions(QChart::AllAnimations);

    //allinea a sinistra la legenda
    chartUomini->legend()->setAlignment(Qt::AlignRight);
    chartDonne->legend()->setAlignment(Qt::AlignRight);
    //connetti on mouse over sugli spicchi dei grafici
    connect(serieDonne, &QPieSeries::hovered, this, &MainWindow::mostraPercentualeSlice);
    connect(serieUomini, &QPieSeries::hovered, this, &MainWindow::mostraPercentualeSlice);

    //collega gli oggetti chart ai grafici nella UI
    ui->QChartDonne->setChart(chartDonne);
    ui->QChartUomini->setChart(chartUomini);
}

void MainWindow::mostraPercentualeSlice(QPieSlice *slice){
    //se la label dello spicchio è visibile allora impostala su non visibile e sostituisci
    //la attuale label con la percentuale con quella originale contenente la fascia d'età
    if(slice->isLabelVisible()==true){
        slice->setLabelVisible(false);
        slice->setLabel(label_iniziale);
    }else{
        //se non è visibile prendine la percentuale (espressa in 0.) e convertila in %
        //e infine mostrala nella label
        label_iniziale = slice->label();
        const qreal percentuale = slice->percentage();
        float perc= (static_cast<float>(percentuale)*100);
        perc = (static_cast<int>(perc * 100 + .5) / 100.0);
        QString p = QString::number(perc);
        slice->setLabel(p + "%");
        slice->setLabelVisible(true);
    }
}
