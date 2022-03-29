#pragma once

#include <QObject>
#include <QUrl>
#include <QWebSocket>

class ClientController : public QObject {
 public:
  explicit ClientController(const QUrl& url);

  public Q_SLOTS:
  void OnConnect();
  void OnDisconnect();

 private:
  QUrl server_url_;
  QWebSocket socket_;
};


