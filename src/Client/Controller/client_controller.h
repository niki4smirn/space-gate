#pragma once

#include <QObject>
#include <QUrl>
#include <QWebSocket>

#include "../../AbstractController/abstract_controller.h"

class ClientController : public AbstractController {
 public:
  explicit ClientController(const QUrl& url);
  ~ClientController() override = default;

  QString GetControllerName() const override;

  public Q_SLOTS:
  void OnConnect();
  void OnDisconnect();

 protected:
  void OnTick() override;
  void Send(const Event& event) override;

 private:
  QUrl server_url_;
  QWebSocket socket_;

  std::vector<Event> events_to_send_;
};


