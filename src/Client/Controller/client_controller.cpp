#include "client_controller.h"

ClientController::ClientController(const QUrl& url) :
    server_url_(url),
    view_(new ClientView) {
  LOG << "Connecting to " << url.host();
  connect(&socket_, &QWebSocket::connected, this,
          &ClientController::OnConnect);
  connect(&socket_, &QWebSocket::disconnected, this,
          &ClientController::OnDisconnect);
  connect(&socket_, &QWebSocket::binaryMessageReceived, this,
          &ClientController::OnByteArrayReceived);
  ConnectToServer();
  StartTicking();
  ConnectView();
}

void ClientController::OnConnect() {
  LOG << "Successful connection to " << server_url_;
  view_->ShowMainMenu();
}

void ClientController::OnDisconnect() {
  LOG << "Failed connection to " << server_url_;
  view_->ShowNetworkProblemWidget();
}

QString ClientController::GetControllerName() const {
  return "Client";
}

void ClientController::OnTick() {
}

void ClientController::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);
  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  socket_.sendBinaryMessage(byte_array);
}

void ClientController::Handle(const events::EventWrapper& event) {
  switch (event.type_case()) {
    case events::EventWrapper::kServerEvent: {
      const auto& server_event = event.server_event();
      switch (server_event.type_case()) {
        case server_events::ServerEventWrapper::kRoomInfo: {
          view_->UpdateRoomInfoMenu(server_event.room_info(),
                                    server_event.receiver_id());
          break;
        }
        case server_events::ServerEventWrapper::kRoomsList: {
          view_->UpdateRoomsListMenu(server_event.rooms_list());
          break;
        }
        case server_events::ServerEventWrapper::kStartGame: {
          view_->PlayStartEffect();
        }
        case server_events::ServerEventWrapper::kGameInfo: {
          const auto& game_info = server_event.game_info();
          int minigame_menu_pos = 0;
          minigame_index_to_pos.clear();
          minigame_pos_to_index.clear();
          for (const auto& minigame_info : game_info.minigames_info()) {
            minigame_index_to_pos[minigame_info.id()] = minigame_menu_pos;
            minigame_pos_to_index[minigame_menu_pos] = minigame_info.id();
            ++minigame_menu_pos;
          }
          view_->UpdateProgress(game_info.progress());
          for (const auto& minigame_info : game_info.minigames_info()) {
            auto minigame_pos = MinigamePosById(minigame_info.id());
            if (minigame_pos) {
              view_->UpdateMinigameBulbs(minigame_pos.value(),
                                         minigame_info.num_of_joined());
            }
          }
          break;
        }
        default: {}
      }
      break;
    }
    default: {}
  }
}

void ClientController::OnByteArrayReceived(const QByteArray& message) {
  events::EventWrapper received_event;
  if (!received_event.ParseFromArray(message.data(), message.size())) {
    LOG << GetControllerName() << " failed to parse message";
    return;
  }
  AddEventToHandle(received_event);
  LogEvent(received_event, logging::Type::kReceive);
}

void ClientController::ConnectView() {
  connect(view_,
          &ClientView::ReadyButtonPressed,
          this,
          &ClientController::SendReadyStatus);
  connect(view_,
          &ClientView::CreateRoom,
          this,
          &ClientController::SendCreateRoomEvent);
  connect(view_,
          &ClientView::LeaveRoom,
          this,
          &ClientController::SendLeaveRoomEvent);
  connect(view_,
          &ClientView::JoinRoom,
          this,
          &ClientController::SendJoinRoomEvent);
  connect(view_,
          &ClientView::StartGame,
          this,
          &ClientController::SendStartGameEvent);
  connect(view_,
          &ClientView::KeyEventToServer,
          this,
          &ClientController::SendKeyEvent);
  connect(view_,
          &ClientView::MouseMoveToServer,
          this,
          &ClientController::SendMouseMoveEvent);
  connect(view_,
          &ClientView::JoinMinigame,
          [&](int minigame_menu_pos) {
            int minigame_id = MinigameIdByPos(minigame_menu_pos);
            if (minigame_id != 0) {
              SendJoinMinigame(minigame_id);
            }
  });
  connect(view_,
          &ClientView::LeaveMinigame,
          this,
          &ClientController::SendLeaveMinigame);
  connect(view_,
          &ClientView::Reconnect,
          [&]() {
            ConnectToServer();
  });
}

void ClientController::SendReadyStatus() {
  auto* wait_status_event = new client_events::ChangeWaitingStatus;
  auto* event_to_room = new client_events::EventToRoom;
  event_to_room->set_allocated_change_waiting_status(wait_status_event);

  auto* event_wrapper = new client_events::ClientEventWrapper;
  event_wrapper->set_allocated_event_to_room(event_to_room);

  events::EventWrapper ready_event;
  ready_event.set_allocated_client_event(event_wrapper);
  AddEventToSend(ready_event);
}

void ClientController::SendCreateRoomEvent() {
  auto* create_room_event = new client_events::CreateRoom;
  auto* event_to_server = new client_events::EventToServer;
  event_to_server->set_allocated_create_room(create_room_event);

  auto* client_event_wrapper = new client_events::ClientEventWrapper;
  client_event_wrapper->set_allocated_event_to_server(event_to_server);

  events::EventWrapper event_to_send;
  event_to_send.set_allocated_client_event(client_event_wrapper);
  AddEventToSend(event_to_send);
}

void ClientController::SendLeaveRoomEvent() {
  auto* leave_room_event = new client_events::LeaveRoom;
  auto* event_to_server = new client_events::EventToServer;
  event_to_server->set_allocated_leave_room(leave_room_event);

  auto* client_event_wrapper = new client_events::ClientEventWrapper;
  client_event_wrapper->set_allocated_event_to_server(event_to_server);

  events::EventWrapper event_to_send;
  event_to_send.set_allocated_client_event(client_event_wrapper);
  AddEventToSend(event_to_send);
}

void ClientController::SendJoinRoomEvent(RoomId room_id) {
  auto* join_room_event = new client_events::EnterRoom;
  join_room_event->set_room_id(room_id);

  auto* event_to_server = new client_events::EventToServer;
  event_to_server->set_allocated_enter_room(join_room_event);

  auto* client_event_wrapper = new client_events::ClientEventWrapper;
  client_event_wrapper->set_allocated_event_to_server(event_to_server);

  events::EventWrapper event_to_send;
  event_to_send.set_allocated_client_event(client_event_wrapper);
  AddEventToSend(event_to_send);
}

void ClientController::SendStartGameEvent() {
  auto* start_game_event = new client_events::StartGame;
  auto* event_to_room = new client_events::EventToRoom;
  event_to_room->set_allocated_start_game(start_game_event);

  auto* event_wrapper = new client_events::ClientEventWrapper;
  event_wrapper->set_allocated_event_to_room(event_to_room);

  events::EventWrapper start_event;
  start_event.set_allocated_client_event(event_wrapper);
  AddEventToSend(start_event);
}

void ClientController::SendKeyEvent(input::Name key) {
  LOG << input::InputNameToString(key);
}

void ClientController::SendMouseMoveEvent(const QPoint& pos) {
  LOG << "x = " << pos.x() << " y = " << pos.y();
}

void ClientController::SendJoinMinigame(int minigame_index) {
  auto* join_minigame_event = new client_events::JoinMinigame;
  join_minigame_event->set_minigame_id(minigame_index);

  auto* event_to_game = new client_events::EventToGame;
  event_to_game->set_allocated_join_minigame(join_minigame_event);

  auto* event_wrapper = new client_events::ClientEventWrapper;
  event_wrapper->set_allocated_event_to_game(event_to_game);

  events::EventWrapper event;
  event.set_allocated_client_event(event_wrapper);
  AddEventToSend(event);
}

void ClientController::SendLeaveMinigame() {
  auto* leave_minigame_event = new client_events::LeaveMinigame;

  auto* event_to_game = new client_events::EventToGame;
  event_to_game->set_allocated_leave_minigame(leave_minigame_event);

  auto* event_wrapper = new client_events::ClientEventWrapper;
  event_wrapper->set_allocated_event_to_game(event_to_game);

  events::EventWrapper event;
  event.set_allocated_client_event(event_wrapper);
  AddEventToSend(event);
}

int ClientController::MinigameIdByPos(int minigame_pos) {
  if (!minigame_pos_to_index.contains(minigame_pos)) {
    return 0;
  }
  return minigame_pos_to_index[minigame_pos];
}
std::optional<int> ClientController::MinigamePosById(int minigame_id) {
  if (!minigame_index_to_pos.contains(minigame_id)) {
    return std::nullopt;
  }
  return {minigame_index_to_pos[minigame_id]};
}

void ClientController::ConnectToServer() {
  socket_.open(server_url_);
}
