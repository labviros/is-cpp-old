#ifndef __IS_CONNECTION_HPP__
#define __IS_CONNECTION_HPP__

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <chrono>
#include <string>
#include <vector>
#include "helpers.hpp"

namespace is {

using namespace AmqpClient;

struct Connection {
  Channel::ptr_t channel;
  const std::string exchange;

  Connection(std::string const& uri, std::string const& exchange = "data") : Connection(make_channel(uri), exchange) {}

  Connection(Channel::ptr_t channel, std::string const& exchange = "data") : channel(channel), exchange(exchange) {
    // passive durable auto_delete
    channel->DeclareExchange(exchange, Channel::EXCHANGE_TYPE_TOPIC, false, false, false);
  }

  void publish(std::string const& topic, BasicMessage::ptr_t message) {
    bool mandatory{false};
    if (!message->TimestampIsSet()) {
      set_timestamp(message);
    }
    channel->BasicPublish(exchange, topic, message, mandatory);
  }

  std::string subscribe(std::vector<std::string> const& topics) {
    // queue_name, passive, durable, exclusive, auto_delete
    Table arguments{{TableKey("x-max-length"), TableValue(1)}};
    auto queue = channel->DeclareQueue("", false, false, true, true, arguments);

    for (auto& topic : topics) {
      channel->BindQueue(queue, exchange, topic);
    }

    // no_local, no_ack, exclusive
    return channel->BasicConsume(queue, "", true, true, true);
  }

  Envelope::ptr_t consume(std::string const& tag) {
    Envelope::ptr_t envelope = channel->BasicConsumeMessage(tag);
    return envelope;
  }

  template <typename Time>
  Envelope::ptr_t consume_for(std::string const& tag, Time const& timeout) {
    using namespace std::chrono;
    int timeout_ms = duration_cast<milliseconds>(timeout).count();
    Envelope::ptr_t envelope;
    channel->BasicConsumeMessage(tag, envelope, timeout_ms);
    return envelope;
  }
};

}  // ::is

#endif  // __IS_CONNECTION_HPP__