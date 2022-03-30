#pragma once

#include "../../AbstractController/abstract_controller.h"
#include "../Models/RoomModel/room_model.h"

class RoomController : public AbstractController {
 public:
  RoomController(RoomId room_id, const std::shared_ptr<User>& host);
  QString GetControllerName() const override;

  RoomId GetId() const;
  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);

 protected:
  void OnTick() override;

  void Send(const proto::Event& event) override;

 private:
  RoomModel room_model_;
};
