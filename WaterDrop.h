#include <QtWidgets/QWidget>
#include <QVariantAnimation>
#include <QMainWindow>
class WaterDrop : public QMainWindow
{
    Q_OBJECT

public:
    WaterDrop(QWidget *parent = Q_NULLPTR);
    ~WaterDrop();
    void show();
    void move(const QPoint &point);
    void setColor(QColor color);

private:
    void paintEvent(QPaintEvent *event);
public slots:
    void onRaduisChanged(QVariant value);

private:
    QVariantAnimation* m_waterDropAnimation;
    // 水滴变化的半径;
    int m_animationRadius;
    // 水滴的颜色;
    QColor m_waterDropColor;
};
