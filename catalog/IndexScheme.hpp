/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_CATALOG_INDEX_SCHEME_HPP_
#define QUICKSTEP_CATALOG_INDEX_SCHEME_HPP_

#include <algorithm>
#include <cstddef>
#include <string>
#include <unordered_map>

#include "catalog/Catalog.pb.h"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief The Index Scheme class which stores the information about
 *        various indicies defined for a particular relation.
 **/
class IndexScheme {
 public:
  /**
   * @brief Constructor.
   **/
  IndexScheme() {
  }

  /**
   * @brief Reconstruct an Index Scheme from its serialized
   *        Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a Index Scheme,
   *              previously produced by getProto().
   * @return The deserialied index scheme object.
   **/
  static IndexScheme* ReconstructFromProto(const serialization::IndexScheme &proto);

  /**
   * @brief Check whether a serialization::IndexScheme is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a
   *              IndexScheme, originally generated by getProto().
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::IndexScheme &proto);

  /**
   * @brief Serialize the Index Scheme as Protocol Buffer.
   *
   * @return The Protocol Buffer representation of Index Scheme.
   **/
  serialization::IndexScheme getProto() const;

  /**
   * @brief Get the number of indices for the relation.
   *
   * @return The number of indices defined for the relation.
   **/
  inline std::size_t getNumIndices() const {
    return index_map_.size();
  }

  /**
   * @brief Check whether an index with the given exists or not.
   *
   * @param index_name Name of the index to be checked.
   * @return Whether the index exists or not.
   **/
  bool hasIndexWithName(const std::string &index_name) const {
    return index_map_.find(index_name) != index_map_.end();
  }

  /**
   * @brief Check whether an index with the given description
   *        containing the same attribute id and index type
   *        exists or not in the index map.
   *
   * @param index_descripton Index Description to check against.
   * @return Whether a similar index description was already defined or not.
   **/
  bool hasIndexWithDescription(const IndexSubBlockDescription &index_description_checked) const {
    // Iterate through every index description corresponding to each key in the index map.
    for (auto cit = index_map_.cbegin(); cit != index_map_.cend(); ++cit) {
      const IndexSubBlockDescription &index_description_expected = cit->second;
      // Check if the stored description matches the given description.
      if (areIndexDescriptionsSame(index_description_expected, index_description_checked)) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Check whether two index descriptions are same or not.
   *        Two index descriptions are same if they have any matching
   *        attributes ids and same index type.
   *
   * @param description_expected First index description.
   * @param description_checked Second index description.
   * @return Whether the two index_descriptions are similar or not.
   **/
  bool areIndexDescriptionsSame(const IndexSubBlockDescription &description_expected,
                                const IndexSubBlockDescription &description_checked) const {
    if (description_expected.sub_block_type() != description_checked.sub_block_type()) {
      return false;
    }

    // Serialize and sort the two protobuf index descriptions and compare.
    std::string serialized_description_expected, serialized_description_checked;
    description_expected.SerializeToString(&serialized_description_expected);
    description_checked.SerializeToString(&serialized_description_checked);
    std::sort(serialized_description_expected.begin(), serialized_description_expected.end());
    std::sort(serialized_description_checked.begin(), serialized_description_checked.end());

    return (serialized_description_expected.compare(serialized_description_checked) == 0);
  }

  /**
   * @brief Adds a new index entry to the index map.
   * @warning Must call before hasIndexWithName() and hasIndexWithDescription().
   * @note This method assumes that the caller has already acquired the
   *       necessary locks before invoking it.
   *
   * @param index_name The name of index to add (key).
   * @param index_description The index description for this index (value).
   **/
  bool addIndexMapEntry(const std::string &index_name,
                        IndexSubBlockDescription &&index_description) {  // NOLINT(whitespace/operators)
    if (index_map_.find(index_name) != index_map_.end()) {
      return false;  // index_name is already present!
    }
    // Value for this index_map key is the index description provided.
    index_map_.emplace(index_name, std::move(index_description));
    return true;
  }

 private:
  // A map of index names to their index description.
  std::unordered_map<std::string, IndexSubBlockDescription> index_map_;

  DISALLOW_COPY_AND_ASSIGN(IndexScheme);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_INDEX_SCHEME_HPP_
