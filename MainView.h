#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

class MainView : public QWidget {
	Q_OBJECT
private:
	QBrush bg_brush_;
protected:
	void paintEvent(QPaintEvent *) override;
public:
	explicit MainView(QWidget *parent = nullptr);
};

#endif // MAINVIEW_H
