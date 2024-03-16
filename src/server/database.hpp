int getNextSequence(mongocxx::client &client, const std::string &db_name,
                    const std::string &sequence_collection_name) {
  auto collection = client[db_name][sequence_collection_name];
  bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
      collection.find_one_and_update(
          bsoncxx::builder::stream::document{}
              << bsoncxx::builder::stream::finalize,
          bsoncxx::builder::stream::document{}
              << "$inc" << bsoncxx::builder::stream::open_document << "seq" << 1
              << bsoncxx::builder::stream::close_document
              << bsoncxx::builder::stream::finalize,
          mongocxx::options::find_one_and_update{}.upsert(true));

  if (maybe_result) {
    auto doc = maybe_result.value().view();
    if (doc["seq"]) {
      return doc["seq"].get_int32().value;
    }
  }
  return 0; // Default value if sequence is not found
}