#ifndef CONFIGUREVIEW_H
#define CONFIGUREVIEW_H

#include <QWidget>
#include "ui_ConfigureView.h"

class ConfigureView : public QWidget
{
	Q_OBJECT

public:
	ConfigureView(QWidget *parent = 0);
	~ConfigureView();

private:
	Ui::ConfigureView ui;
};

#endif // CONFIGUREVIEW_H
