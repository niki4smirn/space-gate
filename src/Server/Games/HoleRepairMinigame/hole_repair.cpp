#include "hole_repair.h"

using namespace hole_repair_settings;

HoleRepair::HoleRepair(const std::vector<std::shared_ptr<User>>& players) :
    AbstractMinigame(players, 1000, kGameDuration) {
  StartMinigame();
}

void HoleRepair::ReceiveMousePos(QPointF pos) {
  events::EventWrapper event;
  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_receiver_id(player_id_by_role_id_.at(0));

  auto* minigame_response = new minigame_responses::MinigameResponse;
  minigame_response->set_remaining_time(duration_ - ticks_);

  auto* hole_repair_response = new minigame_responses::HoleRepairResponse;
  auto* point = new minigame_responses::Point;
  point->set_x(pos.x());
  point->set_y(pos.y());
  hole_repair_response->set_allocated_mouse_pos(point);

  minigame_response->set_allocated_hole_repair_response(hole_repair_response);

  server_event->set_allocated_game_response(minigame_response);

  event.set_allocated_server_event(server_event);

  AddEventToSend(event);
}

void HoleRepair::ReceivePlatePos(QPointF pos) {
  plates_.push_back(pos);
  events::EventWrapper event;
  auto* server_event = new server_events::ServerEventWrapper;
  server_event->set_receiver_id(player_id_by_role_id_.at(0));

  auto* minigame_response = new minigame_responses::MinigameResponse;
  minigame_response->set_remaining_time(duration_ - ticks_);

  auto* hole_repair_response = new minigame_responses::HoleRepairResponse;
  auto* point = new minigame_responses::Point;
  point->set_x(pos.x());
  point->set_y(pos.y());
  hole_repair_response->set_allocated_plate_pos(point);

  minigame_response->set_allocated_hole_repair_response(hole_repair_response);

  server_event->set_allocated_game_response(minigame_response);

  event.set_allocated_server_event(server_event);

  AddEventToSend(event);
}

void HoleRepair::StartMinigame() {
  AbstractMinigame::StartMinigame();
  GenerateHoles();
  SendInitialResponse();
}

QString HoleRepair::GetControllerName() const {
  return "Hole Repair Minigame";
}

void HoleRepair::Send(const events::EventWrapper& event) {
  LogEvent(event, logging::Type::kSend);

  auto receiver_id = event.server_event().receiver_id();

  auto& receiver = players_[receiver_id];

  auto serialized = event.SerializeAsString();
  QByteArray byte_array(serialized.data(), serialized.size());
  receiver->GetSocket()->sendBinaryMessage(byte_array);
}

void HoleRepair::Handle(const events::EventWrapper& event) {
  const auto& action =
      event.client_event().event_to_game().minigame_action()
          .hole_repair_minigame();
  switch (action.pos_case()) {
    case minigame_actions::HoleRepairMinigame::kMousePos: {
      ReceiveMousePos(QPointF(action.mouse_pos().x(),
                              action.mouse_pos().y()));
      break;
    }
    case minigame_actions::HoleRepairMinigame::kPlatePos: {
      ReceivePlatePos(QPointF(action.plate_pos().x(), action.plate_pos().y()));
      break;
    }
    default: {};
  }
}

void HoleRepair::SendInitialResponse() {
  for (int i = 0; i < players_.size(); ++i) {
    events::EventWrapper event;
    auto* server_event = new server_events::ServerEventWrapper;
    server_event->set_receiver_id(player_id_by_role_id_.at(i));
    auto* minigame_response = new minigame_responses::MinigameResponse;
    minigame_response->set_remaining_time(duration_);
    auto* initial_hole_repair_response =
        new minigame_responses::InitialHoleRepairResponse;
    initial_hole_repair_response->set_role(static_cast<minigame_responses::
    InitialHoleRepairResponse::Role>(i));
    auto* holes = new minigame_responses::Holes;
    for (int i = 0; i < holes_.size(); ++i) {
      auto* point = holes->add_points();
      point->set_x(holes_.at(i).x());
      point->set_y(holes_.at(i).y());
    }
    initial_hole_repair_response->set_allocated_holes(holes);
    initial_hole_repair_response->set_plates(holes_.size() + kAdditionPlates);
    minigame_response->set_allocated_initial_hole_repair_response(
        initial_hole_repair_response);

    server_event->set_allocated_game_response(minigame_response);
    event.set_allocated_server_event(server_event);

    AddEventToSend(event);
  }
}

events::EventWrapper HoleRepair::GenerateResponseMessage(UserId user_id) {
  events::EventWrapper event;
  return event;
}

void HoleRepair::SendResponseMessages() {}

bool HoleRepair::IsCompleted() {
  if (plates_.size() < holes_.size() + kAdditionPlates) {
    return false;
  }
  return true;
}

void HoleRepair::GenerateHoles() {
  int holes_number;
  if (kIsRandomHolesNumber) {
    holes_number = QRandomGenerator::global()->bounded(
        kMaxHolesNumber - kMinHolesNumber + 1) - kMinHolesNumber;
  } else {
    holes_number = kHolesNumber;
  }
  for (int i = 0; i < holes_number; ++i) {
    holes_.emplace_back(QRandomGenerator::global()->bounded(
                            kRelativeCoords + 1),
                        QRandomGenerator::global()->bounded(
                            kRelativeCoords + 1));
  }
}

bool HoleRepair::Check() {
  int mistakes = 0;
  for (int i = 0; i < plates_.size(); ++i) {
    double min = kRelativeCoords;
    for (int j = 0; j < holes_.size(); ++j) {
      QPointF user_point(plates_.at(i).x(), plates_.at(i).y());
      double length = Length(user_point, holes_.at(j));
      if (min > length) {
        min = length;
      }
    }
    if (min > kMaxDeviation) {
      mistakes++;
    }
  }
  return mistakes < kMaxMistakes;
}

double HoleRepair::Length(QPointF pnt1, QPointF pnt2) {
  return sqrt((pnt1.x() - pnt2.x()) * (pnt1.x() - pnt2.x())
                  + (pnt1.y() - pnt2.y()) * (pnt1.y() - pnt2.y()));
}

void HoleRepair::OnTick() {
  AbstractMinigame::OnTick();
  SendTimeMessage();

  bool is_completed = IsCompleted();

  if (is_completed) {
    if (Check()) {
      emit MinigameEnded(MinigameType::kHoleRepair, max_score_);
    } else {
      emit MinigameEnded(MinigameType::kHoleRepair, 0);
    }
  } else if (ticks_ >= duration_) {
    emit MinigameEnded(MinigameType::kHoleRepair, 0);
  }
}

void HoleRepair::SendTimeMessage() {
  for (int i = 0; i < players_.size(); ++i) {
    events::EventWrapper event;
    auto* server_event = new server_events::ServerEventWrapper;
    server_event->set_receiver_id(player_id_by_role_id_.at(i));
    auto* minigame_response = new minigame_responses::MinigameResponse;
    minigame_response->set_remaining_time(duration_ - ticks_);
    auto* hole_repair_response = new minigame_responses::HoleRepairResponse;
    minigame_response->set_allocated_hole_repair_response(
        hole_repair_response);

    server_event->set_allocated_game_response(minigame_response);
    event.set_allocated_server_event(server_event);

    AddEventToSend(event);
  }
}
