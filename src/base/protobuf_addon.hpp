char buf[5000];
void Session::Read() {
  boost::asio::async_read(socket, boost::asio::buffer(buf),
                          boost::asio::transfer_at_least(1),
                          boost::bind(&Session::Handle_Read, shared_from_this(),
                                      boost::asio::placeholders::error));
}
void Session::Handle_Read(const boost::system::error_code &error) {
  if (!error) {
    google::protobuf::io::ArrayInputStream arrayInputStream(buf, 5000);
    google::protobuf::io::CodedInputStream codedInputStream(&arrayInputStream);
    uint32_t messageSize;
    codedInputStream.ReadVarint32(&messageSize);
    // Read more here
    MyProtoMsg myProtoMsg;
    myProtoMsg.ParseFromCodedStream(&codedInputStream);
  }
  Read();
}