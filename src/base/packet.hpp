#include "action.pb.h"
#include <cstdint>
#include <iostream>

enum WhiteboardPacketType {
  // client packet
  create_whiteboard,
  create_share_url,
  quit_whiteboard,
  add_element,
  modify_element,
  delete_element,
  // server packet
  broadcast,
  confirm_action,
  error
};

class WhiteboardPacket {
private:
  uint8_t version;
  uint8_t type;
  uint8_t session_id;
  uint8_t user_id;

public:
  WhiteboardPacket(uint8_t _version = 1,
                   uint8_t _type = WhiteboardPacketType::create_whiteboard,
                   uint8_t _session_id = 0, uint8_t _user_id = 0)
      : version(_version), type(_type), session_id(_session_id),
        user_id(_user_id) {}

  void new_create_whiteboard();
  void new_create_share_url();
  void new_quit_whiteboard();
  void new_add_element();
  void new_modify_element();
  void new_delete_element();
  void new_broadcast();
  void new_confirm_action();
  void new_error();
};