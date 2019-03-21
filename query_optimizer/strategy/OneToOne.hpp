/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       https://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_ONE_TO_ONE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_ONE_TO_ONE_HPP_

#include <string>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/strategy/Strategy.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class LogicalToPhysicalMapper;

namespace strategy {

/** \addtogroup OptimizerStrategy
 *  @{
 */

/**
 * @brief Simple strategy that converts one logical node to one physical node.
 */
class OneToOne : public Strategy {
 public:
  /**
   * @brief Constructor.
   *
   * @param physical_mapper The physical plan generator used to get child
   *        physical plans.
   */
  explicit OneToOne(LogicalToPhysicalMapper *physical_mapper)
      : Strategy(physical_mapper) {}

  std::string getName() const override { return "OneToOne"; }

  bool generatePlan(const logical::LogicalPtr &logical_input,
                    physical::PhysicalPtr *physical_output) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(OneToOne);
};

/** @} */

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_ONE_TO_ONE_HPP_ */
