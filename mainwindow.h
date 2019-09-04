#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt includes.
#include <QAudioInput>
#include <QAudioOutput>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_refresh_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_horizontalSlider_input_level_valueChanged(int);

    void on_horizontalSlider_output_level_valueChanged(int);

private:
    Ui::MainWindow* ui;

    QList<QAudioDeviceInfo> input_device_list;
    QList<QAudioDeviceInfo> output_device_list;

    QAudioInput* audio_in = nullptr;
    QAudioOutput* audio_out = nullptr;
};

#endif // MAINWINDOW_H
