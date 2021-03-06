// Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "paddle/fluid/framework/ir/graph_traits.h"

#include <vector>

namespace paddle {
namespace framework {
namespace ir {

//
// NodesDFSIterator
//
NodesDFSIterator::NodesDFSIterator(const std::vector<Node *> &source) {
  for (auto *x : source) stack_.push(x);
}

NodesDFSIterator::NodesDFSIterator(NodesDFSIterator &&other) noexcept
    : stack_(std::move(other.stack_)),
      visited_(std::move(other.visited_)) {}

NodesDFSIterator::NodesDFSIterator(const NodesDFSIterator &other)
    : stack_(other.stack_), visited_(other.visited_) {}

Node &NodesDFSIterator::operator*() {
  PADDLE_ENFORCE(!stack_.empty());
  return *stack_.top();
}

NodesDFSIterator &NodesDFSIterator::operator++() {
  PADDLE_ENFORCE(!stack_.empty(), "the iterator exceeds range");
  visited_.insert(stack_.top());
  auto *cur = stack_.top();
  stack_.pop();
  for (auto *x : cur->outputs) {
    if (!visited_.count(x)) {
      stack_.push(x);
    }
  }
  return *this;
}
bool NodesDFSIterator::operator==(const NodesDFSIterator &other) {
  if (stack_.empty()) return other.stack_.empty();
  if ((!stack_.empty()) && (!other.stack_.empty())) {
    return stack_.top() == other.stack_.top();
  }
  return false;
}

NodesDFSIterator &NodesDFSIterator::operator=(const NodesDFSIterator &other) {
  stack_ = other.stack_;
  visited_ = other.visited_;
  return *this;
}
Node *NodesDFSIterator::operator->() { return stack_.top(); }

}  // namespace ir
}  // namespace framework
}  // namespace paddle
