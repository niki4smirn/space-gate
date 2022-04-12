#pragma once

#include "src/AbstractController/abstract_controller.h"
#include "src/Server/Models/RoomModel/room_model.h"

class RoomController : public AbstractController {
 public:
  RoomController(RoomId room_id, const std::shared_ptr<User>& chief);
  QString GetControllerName() const override;

  RoomId GetId() const;
  void AddUser(const std::shared_ptr<User>& user);
  void DeleteUser(UserId id);

  bool IsEmpty() const;

 protected:
  void OnTick() override;

  void Send(const events::Wrapper& event) override;
  void Handle(const events::Wrapper& event) override;

 private:
  RoomModel room_model_;
};
