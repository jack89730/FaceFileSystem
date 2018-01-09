#include "QCustomCheckBox.h"

QCustomCheckBox::QCustomCheckBox(int index,QWidget* parent):
    QCheckBox(parent)
{
    m_index = index;
    connect(this,SIGNAL(clicked(bool)),this,SLOT(slots_custom_checkbox_cliked(bool)));
}

int QCustomCheckBox::getIndex()
{
    return m_index;

}

void QCustomCheckBox::slots_custom_checkbox_cliked(bool status)
{
    emit(signal_custom_checkbox_clicked(m_index,status));
}

QCustomCheckBox::~QCustomCheckBox()
{

}
