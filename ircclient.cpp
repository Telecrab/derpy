#include "ircclient.h"
#include <QByteArray>
#include <QDebug>
#include <QRegularExpression>
#include <QThread>
#include <QCoreApplication>
#include <QMap>
#include <QPair>

// Literals to send.
const char IrcClient::PASS[]    = "PASS %1\r\n";
const char IrcClient::NICK[]    = "NICK %1\r\n";
const char IrcClient::TAGS[]    = "CAP REQ :twitch.tv/tags\r\n";
const char IrcClient::JOIN[]    = "JOIN #%1\r\n";
const char IrcClient::PONG[]    = "PONG tmi.twitch.tv\r\n";
const char IrcClient::PRIVMSG[] = "PRIVMSG #%1 :";

// Literals to compare to.
const char IrcClient::LOGIN_SUCCESS[]   = ":tmi.twitch.tv 376 %1 :>\r";
const char IrcClient::PING[]            = "PING :tmi.twitch.tv\r";

IrcClient::IrcClient(QObject *parent)
    : QObject(parent),
      m_irc(new QTcpSocket(this))
{
    connect(m_irc, &QTcpSocket::readyRead, this, &IrcClient::dataReceived);
    connect(m_irc, &QTcpSocket::stateChanged, this, &IrcClient::displayConnectionStatus);
    m_irc->connectToHost("irc.twitch.tv", 6667);
}

void IrcClient::dataReceived()
{
    QByteArray data = m_irc->readAll();
    QList<QByteArray> lines = data.split('\n');

    foreach(const QByteArray &line, lines) {

        qDebug() << line;
        if(line == QString(LOGIN_SUCCESS).arg(m_nick).toLatin1()) {
            sendMessage(TAGS);
            emit connected();
            continue;
        }

        if(line == PING) {
            sendMessage(PONG);
            continue;
        }

        QRegularExpression regExp("@color=(.*?);display-name=(.*?);emotes=(.*?);subscriber=([01]);turbo=([01]);user-id=(.*?);user-type=(.*?) :(.*?)!\\8@\\8.tmi.twitch.tv PRIVMSG (.*?) :(.*)\\r");
        QRegularExpressionMatch match = regExp.match(line);
        if(match.hasMatch()) {
            QString message = match.captured(10);

            QRegularExpression regExpEmotes("(\\d+?):((?:\\d+?-\\d+,?)+)\\/?");
            QRegularExpressionMatchIterator emoteIterator = regExpEmotes.globalMatch(match.captured(3));
            QMap< int, QPair<int, QString> > emoteRanges; // QMap< rangeStart, <rangeEnd, emoteCode> > Yeah, it's ugly.

            // Extract the ranges for each emote code.
            while (emoteIterator.hasNext()) {

                QRegularExpressionMatch matchEmotes = emoteIterator.next();

                QRegularExpression regExpIndices("(\\d+)?-(\\d+),?");
                QRegularExpressionMatchIterator rangeIterator = regExpIndices.globalMatch(matchEmotes.captured(2));

                // Convert each range to the pair of integers.
                while (rangeIterator.hasNext()) {
                    QRegularExpressionMatch matchIndices = rangeIterator.next();

                    int start = matchIndices.captured(1).toInt();
                    int end = matchIndices.captured(2).toInt();
                    emoteRanges[start] = QPair<int, QString>(end - start + 1, matchEmotes.captured(1)); // Save the range and emote code to QMap for autosorting by the range beginning.
                }
            }

            message.replace(QRegularExpression("((?:https?|ftp)://\\S+)"), "<a href=\"\\1\">\\1</a>"); // Make URLs clickable.

            // Now replace every emote code with the HTML image tag from the last to the first.
            auto emoteRangeIterator = emoteRanges.constEnd() - 1;
            for(; emoteRangeIterator != emoteRanges.constBegin() - 1; --emoteRangeIterator) {
                message.replace(emoteRangeIterator.key(), emoteRangeIterator.value().first, QStringLiteral("<img src=\"http://static-cdn.jtvnw.net/emoticons/v1/%1/1.0\">").arg( emoteRangeIterator.value().second ) );
            }

            emit newMessage(message, match.captured(2), match.captured(1));

            if(message.contains(m_raffleCommand)) {
                addParticipant( match.captured(2).toLocal8Bit(), match.captured(1) );
                continue;
            }
        }
    }
}

void IrcClient::displayConnectionStatus(QAbstractSocket::SocketState state)
{
    qDebug() << state;
    if(state == QAbstractSocket::ConnectedState) {
        m_irc->write(QString(PASS).arg(m_oauthToken).toLatin1());
        m_irc->write(QString(NICK).arg(m_nick).toLatin1());
    }
}

void IrcClient::joinChannel(QString channel)
{
    sendMessage(QString(JOIN).arg(channel).toLatin1());
}

void IrcClient::setRaffleCommand(QString command)
{
    if(m_raffleCommand != command) {
        m_raffleCommand = command;
        emit raffleCommandChanged(m_raffleCommand);
    }
}

void IrcClient::setNick(QString nick)
{
    if (m_nick == nick)
        return;

    m_nick = nick;
    emit nickChanged(nick);
}

void IrcClient::setOauthToken(QString oauthToken)
{
    if (m_oauthToken == oauthToken)
        return;

    m_oauthToken = oauthToken;
    emit oauthTokenChanged(oauthToken);
}

void IrcClient::addParticipant(QByteArray name, QString colorCode)
{
    if(m_participants.contains(name)) {
        return;
    }

    m_participants << name;
    emit newParticipant(name, colorCode);
}

void IrcClient::sendMessage(QByteArray message)
{
    qDebug() << "======" << message;
    qDebug() << "------" << m_irc->bytesToWrite();
    m_irc->write(message);
    qDebug() << "++++++" << m_irc->bytesToWrite();
}

