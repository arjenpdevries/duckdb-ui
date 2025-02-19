#include "utils/serialization.hpp"

#include "duckdb/common/serializer/deserializer.hpp"
#include "duckdb/common/serializer/serializer.hpp"

namespace duckdb {
namespace ui {

void EmptyResult::Serialize(Serializer &) const {}

void TokenizeResult::Serialize(Serializer &serializer) const {
  serializer.WriteProperty(100, "offsets", offsets);
  serializer.WriteProperty(101, "types", types);
}

// Adapted from parts of DataChunk::Serialize
void ColumnNamesAndTypes::Serialize(Serializer &serializer) const {
  serializer.WriteProperty(100, "names", names);
  serializer.WriteProperty(101, "types", types);
}

// Adapted from parts of DataChunk::Serialize
void Chunk::Serialize(Serializer &serializer) const {
  serializer.WriteProperty(100, "row_count", row_count);
  serializer.WriteList(101, "vectors", vectors.size(),
                       [&](Serializer::List &list, idx_t i) {
                         list.WriteObject([&](Serializer &object) {
                           // Reference the vector to avoid potentially mutating
                           // it during serialization
                           Vector serialized_vector(vectors[i].GetType());
                           serialized_vector.Reference(vectors[i]);
                           serialized_vector.Serialize(object, row_count);
                         });
                       });
}

void SuccessResult::Serialize(Serializer &serializer) const {
  serializer.WriteProperty(100, "success", true);
  serializer.WriteProperty(101, "column_names_and_types",
                           column_names_and_types);
  serializer.WriteList(
      102, "chunks", chunks.size(),
      [&](Serializer::List &list, idx_t i) { list.WriteElement(chunks[i]); });
}

void ErrorResult::Serialize(Serializer &serializer) const {
  serializer.WriteProperty(100, "success", false);
  serializer.WriteProperty(101, "error", error);
}

} // namespace ui
} // namespace duckdb
