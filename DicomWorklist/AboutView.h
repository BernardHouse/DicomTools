#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QWidget>
#include "ui_AboutView.h"

class AboutView : public QWidget
{
	Q_OBJECT

public:
	AboutView(QWidget *parent = 0);
	~AboutView();

private:
	Ui::AboutView ui;
};

#endif // ABOUTVIEW_H
