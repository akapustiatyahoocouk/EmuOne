//
//  emuone-360/Ibm2741FullScreenWidget.cpp
//
//  Ibm2741FullScreenWidget class implementation
//
//////////
#include "emuone-ibm360/API.hpp"
#include "ui_Ibm2741FullScreenWidget.h"
using namespace ibm360;

//////////
//  Construction/destruction
Ibm2741FullScreenWidget::Ibm2741FullScreenWidget(Ibm2741 * ibm2741)
    :   _ui(new Ui::Ibm2741FullScreenWidget),
        _ibm2741(ibm2741)
{
    _ui->setupUi(this);
    Q_ASSERT(_ibm2741 != nullptr);

    this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    _repositionControls();
    _recalculateFont();

    connect(&_refreshTimer, &QTimer::timeout, this, &Ibm2741FullScreenWidget::_refreshTimerTimeout);

    _refreshTimer.start(50);
}

Ibm2741FullScreenWidget::~Ibm2741FullScreenWidget()
{
    _refreshTimer.stop();

    delete _ui;
}


//////////
//  QWidget
void Ibm2741FullScreenWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setFont(_font);
    QFontMetrics fontMetrics = painter.fontMetrics();
    _fontHeight = fontMetrics.height();

    QRect rc = _clientRect();

    switch (_ibm2741->deviceState())
    {
        case Ibm2741::DeviceState::Resetting:
            painter.fillRect(rc, Qt::gray);
            break;
        case Ibm2741::DeviceState::Idle:
            painter.fillRect(rc, QColor(255, 255, 255));
            break;
        case Ibm2741::DeviceState::Reading:
            painter.fillRect(rc, QColor(255, 255, 255));
            painter.setPen(Qt::green);
            painter.drawRect(rc.x(), rc.y(), rc.width(), rc.height());
            break;
        case Ibm2741::DeviceState::Writing:
            painter.fillRect(rc, QColor(255, 255, 255));
            painter.setPen(Qt::red);
            painter.drawRect(rc.x(), rc.y(), rc.width(), rc.height());
            break;
        case Ibm2741::DeviceState::NotOperational:
            painter.fillRect(rc, Qt::black);
            break;
        default:
            Q_ASSERT(false);
    }

    rc = rc.marginsRemoved(QMargins(2, 2, 2, 2));
    int y = rc.y() + fontMetrics.ascent();
    painter.setPen(Qt::black);
    for (int i = 0; i < _ibm2741->_contents.size(); i++)
    {
        QString line = _ibm2741->_contents[i];
        if (i == _ibm2741->_contents.size() - 1)
        {
            line += '_';
        }
        painter.drawText(rc.x(), y, line);
        y += _fontHeight;
    }

    painter.end();
}

void Ibm2741FullScreenWidget::resizeEvent(QResizeEvent * /*event*/)
{
    _repositionControls();
    _recalculateFont();
}

void Ibm2741FullScreenWidget::keyPressEvent(QKeyEvent * event)
{
    if (_ibm2741->deviceState() == Ibm2741::DeviceState::Idle ||
        _ibm2741->deviceState() == Ibm2741::DeviceState::Reading)
    {   //  Accept the input
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {   //  Newline
            _ibm2741->_pendingInput += '\n';
            _ibm2741->_charsToEcho.enqueue('\n');
        }
        else if (event->text().length() == 1 &&
                 event->text()[0].unicode() >= 32 && event->text()[0].unicode() <= 126)
        {   //  Echo
            static util::CharacterSet::Encoder * encoder = util::Cp037CharacterSet::getInstance()->createEncoder();

            _ibm2741->_pendingInput += event->text();
            _ibm2741->_charsToEcho.enqueue(event->text()[0]);
        }
    }
}

//////////
//  core::FullScreenWidget
core::Component * Ibm2741FullScreenWidget::component()
{
    return _ibm2741;
}

QString Ibm2741FullScreenWidget::displayName()
{
    return _ibm2741->name() + " " + _ibm2741->shortStatus();
}

//////////
//  Implementation helpers
QRect Ibm2741FullScreenWidget::_clientRect()
{
    QRect thisGeometry = this->geometry();
    QRect scrollBarGeometry = _ui->_scrollBar->geometry();
    return QRect(thisGeometry.x(),
                 thisGeometry.y(),
                 thisGeometry.width() - scrollBarGeometry.width(),
                 thisGeometry.height());
}

void Ibm2741FullScreenWidget::_repositionControls()
{
    QRect thisGeometry = this->geometry();
    QRect scrollBarGeometry = _ui->_scrollBar->geometry();
    _ui->_scrollBar->setGeometry(thisGeometry.width() - scrollBarGeometry.width(),
                                 0,
                                 scrollBarGeometry.width(),
                                 thisGeometry.height());
}

void Ibm2741FullScreenWidget::_recalculateFont()
{
    QRect rc = _clientRect();
    rc = rc.marginsRemoved(QMargins(2, 2, 2, 2));

    //  What's the largest fixed-pitch font that will still allow 80 columns ?
    QString line(80, 'x');
    for (int pointSize = 1; ; pointSize++)
    {
        QFont candidateFont("Monospace", pointSize);
        candidateFont.setStyleHint(QFont::TypeWriter);
        QFontMetrics fontMetrics(candidateFont);
        if (fontMetrics.horizontalAdvance(line) > rc.width())
        {   //  Too wide!
            break;
        }
        _font = candidateFont;
    }
    qDebug() << _font;
}

//////////
//  Event handlers
void Ibm2741FullScreenWidget::_refreshTimerTimeout()
{
    this->repaint();
}

//  End of emuone-360/Ibm2741FullScreenWidget.cpp
