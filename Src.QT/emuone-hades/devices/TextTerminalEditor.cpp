//
//  emuone-hades/devices/TextTerminalEditor.cpp - emuone::hades::devices::TextTerminalEditor class implementation
//
//  EmuOne
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "emuone-hades/API.hpp"
#include "ui_TextTerminalEditor.h"
using namespace emuone::hades::devices;

//////////
//  Construction/destruction
TextTerminalEditor::TextTerminalEditor(
        QWidget * parent,
        TextTerminal * textTerminal
    ) : emuone::core::ComponentEditor(parent, textTerminal),
        _textTerminal(textTerminal),
        _ui(new Ui::TextTerminalEditor)
{
    _ui->setupUi(this);

    //  Populate combo boxes
    for (int i = TextTerminal::MinTerminalNumber; i < TextTerminal::MaxTerminalNumber; i++)
    {
        _ui->terminalNumberComboBox->addItem(
            emuone::util::toString(i),
            QVariant::fromValue(i));
    }
    for (int i = TextTerminal::MinColumns; i < TextTerminal::MaxColumns; i++)
    {
        _ui->columnsComboBox->addItem(
            emuone::util::toString(i),
            QVariant::fromValue(i));
    }

    for (int i = TextTerminal::MinRows; i < TextTerminal::MaxRows; i++)
    {
        _ui->rowsComboBox->addItem(
            emuone::util::toString(i),
            QVariant::fromValue(i));
    }

    //  Done
    _loadControlValues();
    _constructed = true;
}

TextTerminalEditor::~TextTerminalEditor()
{
    delete _ui;
}

//////////
//  emuone::core::ComponentEditor
bool TextTerminalEditor::isValid() const
{
    return TextTerminal::isValidTerminalNumber(_selectedTerminalNumber()) &&
           TextTerminal::isValidRows(_selectedRows()) &&
           TextTerminal::isValidColumns(_selectedColumns());
}

//////////
//  Signal handlers
void TextTerminalEditor::_loadControlValues()
{
    _setSelectedTerminalNumber(_textTerminal->terminalNumber());
    _setSelectedColumns(_textTerminal->columns());
    _setSelectedRows(_textTerminal->rows());
}

void TextTerminalEditor::_saveControlValues() const
{
    if (isValid())
    {
        _textTerminal->setTerminalNumber(_selectedTerminalNumber());
        _textTerminal->setColumns(_selectedColumns());
        _textTerminal->setRows(_selectedRows());
    }
}

int TextTerminalEditor::_selectedTerminalNumber() const
{
    return _ui->terminalNumberComboBox->currentData().value<int>();
}

void TextTerminalEditor::_setSelectedTerminalNumber(int terminalNumber)
{
    for (int i = 0; i < _ui->terminalNumberComboBox->count(); i++)
    {
        if (terminalNumber == _ui->terminalNumberComboBox->itemData(i).value<int>())
        {
            _ui->terminalNumberComboBox->setCurrentIndex(i);
            break;
        }
    }
}

int TextTerminalEditor::_selectedColumns() const
{
    return _ui->columnsComboBox->currentData().value<int>();
}

void TextTerminalEditor::_setSelectedColumns(int columns)
{
    for (int i = 0; i < _ui->columnsComboBox->count(); i++)
    {
        if (columns == _ui->columnsComboBox->itemData(i).value<int>())
        {
            _ui->columnsComboBox->setCurrentIndex(i);
            break;
        }
    }
}

int TextTerminalEditor::_selectedRows() const
{
    return _ui->rowsComboBox->currentData().value<int>();
}

void TextTerminalEditor::_setSelectedRows(int rows)
{
    for (int i = 0; i < _ui->rowsComboBox->count(); i++)
    {
        if (rows == _ui->rowsComboBox->itemData(i).value<int>())
        {
            _ui->rowsComboBox->setCurrentIndex(i);
            break;
        }
    }
}

//////////
//  Signal handlers
void TextTerminalEditor::_terminalNumberComboBoxCurrentIndexChanged(int)
{
    if (_constructed)
    {
        _saveControlValues();
        emit valueChanged();
    }
}

void TextTerminalEditor::_columnsComboBoxCurrentIndexChanged(int)
{
    if (_constructed)
    {
        _saveControlValues();
        emit valueChanged();
    }
}

void TextTerminalEditor::_rowsComboBoxCurrentIndexChanged(int)
{
    if (_constructed)
    {
        _saveControlValues();
        emit valueChanged();
    }
}

//  End of emuone-hades/devices/TextTerminalEditor.cpp
