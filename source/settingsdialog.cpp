#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <recordingdevices.h>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    settings.readAll();
    readSettings();

    on_checkBoxMicMute_clicked();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    //other
    settings.setFramerate(ui->spinBoxfps->value());
    settings.setVideocodec(ui->lineEditvideocodec->text());
    settings.setAudiocodec(ui->lineEditaudiocodec->text());
    settings.setAudiochannels(ui->spinBoxaudiochannels->value());

    //Microphone
    int MicIndex = ui->comboBoxrecording->currentIndex();
    settings.setMicrophonedevice(ui->comboBoxrecording->itemData(MicIndex).toString());

    //Microphone: Mute?
    if(ui->checkBoxMicMute->isChecked())
    {
        settings.setMicrophonemuted("true");
    }
    else
    {
        settings.setMicrophonemuted("false");
    }

    settings.setFilenameBase(ui->lineEditbasename->text());

    //Filename: use date and time
    if(ui->checkBoxbasenametimedate->isChecked())
    {
        settings.setFilenameUsedate("true");
    }
    else
    {
        settings.setFilenameUsedate("false");
    }

    settings.setFilenamePath(ui->lineEditpath->text());

    //Format
    int formatindex = ui->comboBoxFormat->currentIndex();
    settings.setFormat(ui->comboBoxFormat->itemData(formatindex).toString());

    //Language
    int langindex = ui->comboBoxLanguage->currentIndex();
    settings.setLanguage(ui->comboBoxLanguage->itemData(langindex).toString());

    //Writes the data
    settings.writeAll();
    settings.readAll();
}

void SettingsDialog::on_pushButtonRestore_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("..::" + trUtf8("Restore everything to default?")+ "::..");
    msgBox.setInformativeText(trUtf8("If you press Yes everything will be restored to default. \n\n Be aware that this can NOT be undone."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //msgBox.setDetailedText(QString(ui->textEditConsole->toPlainText()));
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setFixedWidth(520);
    int ret = msgBox.exec();

    switch (ret)  {
      case QMessageBox::Yes:
            {
                ui->checkBoxMicMute->setChecked(1);
                ui->comboBoxrecording->setEnabled(0);

                settings.setDefaults();
                readSettings();
              break;
            }
      case QMessageBox::No:
          // Don't Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

void SettingsDialog::readSettings()
{
// -----------------SECTION: Recordingdevice------------------------------
    //Refreshes the list
    recordingdevices.getRecorddevices();

    //Creates the combobox containing the recording devices that are on the machine.
    int index = 0;
    ui->comboBoxrecording->clear();
    foreach(QString device,recordingdevices.RecordDeviceHW)
    {
        ui->comboBoxrecording->addItem(recordingdevices.RecordDeviceDesc[index],device);

        index += 1;
    }

    //Reads the prefered value
    QString recordingdevice = settings.getMicrophonedevice();
    int comboboxIndex = ui->comboBoxrecording->findData(recordingdevice);
    ui->comboBoxrecording->setCurrentIndex(comboboxIndex);

// -----------------SECTION: Format------------------------------
    //Addes the diffent formats to the combox. PLEASE NOTE: the order is important.
    QStringList ItemFormatList;
    ItemFormatList << "mkv";
    ItemFormatList << "avi";


    QStringList ItemDescList;
    ItemDescList << "MKV - Matroska Multimedia Container";
    ItemDescList << "AVI - Audio Video Interleaver";

    int Indexnumber = 0;
    foreach(QString format,ItemFormatList)
    {
        ui->comboBoxFormat->addItem(ItemDescList[Indexnumber],format);
        Indexnumber += 1;
    }

    //Reads the prefered value
    QString formatcfg = settings.getFormat();
    int comboboxIndexfind = ui->comboBoxFormat->findData(formatcfg);
    ui->comboBoxFormat->setCurrentIndex(comboboxIndexfind);

    // -----------------SECTION: Language------------------------------

    //Updates the list
    findLanguages();

    //Sets the prefered
    QString languagecfg = settings.getLanguage();
    int comboboxIndexLanguage = ui->comboBoxLanguage->findData(languagecfg);
    ui->comboBoxLanguage->setCurrentIndex(comboboxIndexLanguage);

    // -----------------SECTION: Microphonemuted------------------------------
    if(settings.getMicrophonemuted() == "false")
    {
        ui->checkBoxMicMute->setChecked(0);
    }

    // -----------------SECTION: Filename use time and date------------------------------
    if(settings.getFilenameUsedate() == "true")
    {
        ui->checkBoxbasenametimedate->setChecked(true);
        ui->lineEditbasename->setEnabled(0);
    }
    else
    {
        ui->checkBoxbasenametimedate->setChecked(false);
        ui->lineEditbasename->setEnabled(1);
    }


    // -----------------SECTION: Other------------------------------
    ui->lineEditaudiocodec->setText(settings.getAudiocodec());
    ui->lineEditvideocodec->setText(settings.getVideocodec());
    ui->spinBoxaudiochannels->setValue(settings.getAudiochannles());
    ui->spinBoxfps->setValue(settings.getFramerate());
    ui->lineEditbasename->setText(settings.getFilenameBase());
    ui->lineEditpath->setText(settings.getFilenamePath());
}

void SettingsDialog::on_pushButtonpathBrowse_clicked()
{
    QString prefileName = settings.getFilenamePath();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         prefileName,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if(!fileName.isEmpty())
    {
        ui->lineEditpath->setText(fileName);
    }
}

void SettingsDialog::on_checkBoxbasenametimedate_clicked()
{
    if(ui->checkBoxbasenametimedate->isChecked())
    {
        ui->lineEditbasename->setEnabled(0);
    }
    else
    {
        ui->lineEditbasename->setEnabled(1);
    }
}

void SettingsDialog::on_checkBoxMicMute_clicked()
{
    if(ui->checkBoxMicMute->isChecked())
    {
        ui->comboBoxrecording->setEnabled(0);
    }
    else
    {
        ui->comboBoxrecording->setEnabled(1);
    }
}

void SettingsDialog::findLanguages()
{
    QDir translationfilepath(":/translations");
    QStringList translationsfiles = translationfilepath.entryList(QDir::Files);
    qDebug() << "Files found:" << translationsfiles;

    ui->comboBoxLanguage->addItem("Use system default","default");
    ui->comboBoxLanguage->addItem("Use original language (English)","STANDARD");

    foreach(QFileInfo file,translationsfiles)
    {
        QString basename = file.baseName();
        QString localending = file.baseName().remove(0,17);

        qDebug() << "Translation basename:" << basename;
        qDebug() << "Translation ending:" << localending;

        ui->comboBoxLanguage->addItem(basename,localending);
    }
}