#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QObject>
#include <QStringList>
#include <QTcpSocket>
#include <QColor>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class IrcClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString raffleCommand READ raffleCommand WRITE setRaffleCommand NOTIFY raffleCommandChanged)
    Q_PROPERTY(QString nick READ nick WRITE setNick NOTIFY nickChanged)
    Q_PROPERTY(QString oauthToken READ oauthToken WRITE setOauthToken NOTIFY oauthTokenChanged)

public:
    explicit IrcClient(QObject *parent = 0);

    QString raffleCommand() const {return m_raffleCommand;}
    QString nick() const {return m_nick;}
    QString oauthToken() const {return m_oauthToken;}

signals:
    void connected();
    void disconnected();
    void newParticipant(QString name, QString colorCode);
    void newMessage(QString message, QString user, QString colorCode);    
    void raffleCommandChanged(QString raffleCommand);    
    void nickChanged(QString nick);
    void oauthTokenChanged(QString oauthToken);

public slots:
    void joinChannel(QString channel);
    void setRaffleCommand(QString command); 
    void setNick(QString nick);
    void setOauthToken(QString oauthToken);

private slots:
    void dataReceived();
    void displayConnectionStatus(QAbstractSocket::SocketState state);

private:
    // Literals to send.
    static const char PASS[];
    static const char NICK[];
    static const char TAGS[];
    static const char JOIN[];
    static const char PONG[];
    static const char PRIVMSG[];

    // Literals to compare to.
    static const char LOGIN_SUCCESS[];
    static const char PING[];

    void addParticipant(QByteArray name, QString colorCode);
    void sendMessage(QByteArray message);

    QTcpSocket *m_irc;
    QStringList m_messages;
    QByteArrayList m_participants;
    QString m_raffleCommand;
    QString m_nick;
    QString m_oauthToken;
};

#endif // IRCCLIENT_H
