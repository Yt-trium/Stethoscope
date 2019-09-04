#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_pushButton_refresh_clicked();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_refresh_clicked()
{
    ui->comboBox_input_device->clear();
    ui->comboBox_output_device->clear();

    input_device_list
        = QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioInput);
    output_device_list
        = QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioOutput);

    // This is a personnal fix to QTBUG-75781, not perfect yet.
    for (int i = 0; i < input_device_list.size() - 1; ++i) {
        for (int j = 0; j < input_device_list.size(); ++j) {
            if (i != j) {
                if (input_device_list.at(i).deviceName()
                    == input_device_list.at(j).deviceName()) {
                    if (input_device_list.at(i).supportedChannelCounts().size()
                        < input_device_list.at(j)
                              .supportedChannelCounts()
                              .size())
                        input_device_list.removeAt(i);
                    else
                        input_device_list.removeAt(j);
                }
            }
        }
    }
    for (int i = 0; i < output_device_list.size() - 1; ++i) {
        for (int j = 0; j < output_device_list.size(); ++j) {
            if (i != j) {
                if (output_device_list.at(i).deviceName()
                    == output_device_list.at(j).deviceName()) {
                    if (output_device_list.at(i).supportedChannelCounts().size()
                        < output_device_list.at(j)
                              .supportedChannelCounts()
                              .size())
                        output_device_list.removeAt(i);
                    else
                        output_device_list.removeAt(j);
                }
            }
        }
    }

    for (int i = 0; i < input_device_list.size(); ++i)
        ui->comboBox_input_device->addItem(
            input_device_list.at(i).deviceName());

    for (int i = 0; i < output_device_list.size(); ++i)
        ui->comboBox_output_device->addItem(
            output_device_list.at(i).deviceName());
}

void MainWindow::on_pushButton_start_clicked()
{
    QAudioDeviceInfo audio_info_in
        = input_device_list.at(ui->comboBox_input_device->currentIndex());
    QAudioDeviceInfo audio_info_out
        = output_device_list.at(ui->comboBox_output_device->currentIndex());

    QAudioFormat audio_format;
    // audio_format.setSampleRate(8000);
    audio_format.setSampleRate(48000);
    audio_format.setChannelCount(1);
    audio_format.setSampleSize(8);
    audio_format.setCodec("audio/pcm");
    audio_format.setByteOrder(QAudioFormat::LittleEndian);
    audio_format.setSampleType(QAudioFormat::UnSignedInt);

    if (!audio_info_in.isFormatSupported(audio_format)) {
        QMessageBox::critical(this, tr("Input device error"),
            tr("Input device does not support format"));
        on_pushButton_refresh_clicked();

        return;
    }
    if (!audio_info_out.isFormatSupported(audio_format)) {
        QMessageBox::critical(this, tr("Output device error"),
            tr("Output device does not support default format"));
        on_pushButton_refresh_clicked();
        return;
    }

    audio_in = new QAudioInput(audio_info_in, audio_format);
    audio_out = new QAudioOutput(audio_info_out, audio_format);

    audio_in->setVolume(ui->horizontalSlider_input_level->value() / 100.0);
    audio_out->setVolume(ui->horizontalSlider_output_level->value() / 100.0);

    audio_out->start(audio_in->start());

    ui->pushButton_refresh->setEnabled(false);
    ui->comboBox_input_device->setEnabled(false);
    ui->comboBox_output_device->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
}

void MainWindow::on_pushButton_stop_clicked()
{
    audio_out->stop();
    audio_in->stop();

    delete audio_in;
    delete audio_out;

    audio_in = nullptr;
    audio_out = nullptr;

    ui->pushButton_refresh->setEnabled(true);
    ui->comboBox_input_device->setEnabled(true);
    ui->comboBox_output_device->setEnabled(true);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
}

void MainWindow::on_horizontalSlider_input_level_valueChanged(int)
{
    if (audio_in != nullptr)
        audio_in->setVolume(ui->horizontalSlider_input_level->value() / 100.0);

    ui->label_input_level_value->setText(
        QString::number(ui->horizontalSlider_input_level->value()));
}

void MainWindow::on_horizontalSlider_output_level_valueChanged(int)
{
    if (audio_out != nullptr)
        audio_out->setVolume(
            ui->horizontalSlider_output_level->value() / 100.0);

    ui->label_output_level_value->setText(
        QString::number(ui->horizontalSlider_output_level->value()));
}
