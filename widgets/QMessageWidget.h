#ifndef MESSAGE_WIDGET
#define MESSAGE_WIDGET

#include <QWidget>

class QLabel;
class QPushButton;

class QMessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QMessageWidget(int pX , int pY, int width , int height ,QWidget *parent = 0);
    ~QMessageWidget();
    // 设置显示文本
    void setText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel *m_pMessageLabel;
};

#endif // MESSAGE_WIDGET
