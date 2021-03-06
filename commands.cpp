
#include "commands.h"

extern MainWindow* mw_one;

DeleteCommand::DeleteCommand(bool writeINI, bool loadINI, QTableWidget* table0, int table0CurrentRow, QTableWidget* table, int row, QString text, QStringList fieldList, QUndoCommand* parent)
{
    Q_UNUSED(parent);
    m_table = table;
    m_table0 = table0;
    m_row = row;
    m_table0CurrentRow = table0CurrentRow;
    m_text = text;

    m_fieldList = fieldList;
    m_loadINI = loadINI;
    m_writeINI = writeINI;

    setText(QObject::tr("Delete") + "  " + text);
}

void DeleteCommand::undo()
{

    mw_one->goTable(m_table);

    if (m_table0 != NULL && !m_loadINI) {
        m_table0->setCurrentCell(m_table0CurrentRow, 0);
    }

    mw_one->loading = true;

    m_table->insertRow(m_row);

    for (int i = 0; i < m_fieldList.count(); i++) {

        QString strItem = m_fieldList.at(i);

        if (strItem.trimmed() == "true" || strItem.trimmed() == "false") {

            mw_one->init_enabled_data(m_table, m_row, i, strItem.trimmed());

        } else {

            m_table->setItem(m_row, i, new QTableWidgetItem(strItem));
        }
    }

    m_table->setCurrentCell(m_row, 0);

    if (m_loadINI)
        mw_one->read_ini(m_table, m_table0, m_table0CurrentRow);

    if (m_table0 != NULL && !m_loadINI) {

        if (m_writeINI)
            mw_one->write_ini(m_table0, m_table, m_table0CurrentRow);
        else
            mw_one->write_value_ini(m_table0, m_table, m_table0CurrentRow);
    }

    mw_one->loading = false;
}

void DeleteCommand::redo()
{

    mw_one->loading = true;

    mw_one->goTable(m_table);

    m_table->removeRow(m_row);

    mw_one->loading = false;
}

AddCommand::AddCommand(QTableWidget* table, int row, int col, QString text, QUndoCommand* parent)
{
    Q_UNUSED(parent);
    m_table = table;
    m_row = row;
    m_col = col;
    m_text = text;

    setText(QObject::tr("Add") + "  " + text);
}

AddCommand::~AddCommand()
{
}

void AddCommand::undo()
{
    m_table->setItem(m_row, m_col, new QTableWidgetItem(""));
}

void AddCommand::redo()
{
    m_table->setItem(m_row, m_col, new QTableWidgetItem(m_text));
}

EditCommand::EditCommand(QString oldText, QTableWidget* table, int row, int col, QString text, QUndoCommand* parent)
{
    Q_UNUSED(parent);
    m_table = table;
    m_row = row;
    m_col = col;
    m_text = text;
    m_oldText = oldText;

    setText(QObject::tr("Edit") + "  " + oldText);
}

EditCommand::~EditCommand()
{
}

void EditCommand::undo()
{
    mw_one->goTable(m_table);

    mw_one->initLineEdit(m_table, m_row, m_col, m_row, m_col);

    mw_one->loading = true;
    mw_one->writeINI = true;
    m_table->setItem(m_row, m_col, new QTableWidgetItem(m_oldText));

    m_table->setCurrentCell(m_row, m_col);
    m_table->setCellWidget(m_row, m_col, mw_one->lineEdit);
    mw_one->lineEdit->setText(m_oldText);

    mw_one->loading = false;
    mw_one->writeINI = false;
}

void EditCommand::redo()
{

    mw_one->goTable(m_table);

    mw_one->initLineEdit(m_table, m_row, m_col, m_row, m_col);

    mw_one->loading = true;
    mw_one->writeINI = true;
    m_table->setItem(m_row, m_col, new QTableWidgetItem(m_text));

    if (mw_one->lineEdit != NULL) {

        m_table->setCurrentCell(m_row, m_col);
        m_table->setCellWidget(m_row, m_col, mw_one->lineEdit);
        mw_one->lineEdit->setText(m_text);
    }

    mw_one->loading = false;
    mw_one->writeINI = false;
}

QString createCommandString(QString cmdStr)
{
    return cmdStr;
}
