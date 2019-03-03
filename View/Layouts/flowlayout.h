#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>
class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent);
    explicit FlowLayout();
    ~FlowLayout() override;

    void addItem(QLayoutItem *item) override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
};

#endif // FLOWLAYOUT_H
